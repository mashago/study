
extern "C"
{
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
}

#include <string>
#include <fstream>
#include <map>

#include "lock.h"
#include "http_util.h"
#include "http_connect.h"
#include "http_request.h"
#include "http_response.h"

#define SERVER_PORT 8080
#define SERVER_IP "0.0.0.0"

#define MAX_CONNECT	2
#define CONNECT_TIMEOUT 60

// global epoll fd
int g_epfd = -1;

int g_free_connect = 0;  // the free connect id
http_connect_t g_conn_list[MAX_CONNECT];
std::map<uint64_t, int> g_uid_map; // uid to connect id

pthread_mutex_t g_lock; // lock g_uid_map

///// CONNECTION UTIL START [

int init_connection_list()
{
	for (int i = 0; i < MAX_CONNECT; i++)
	{
		g_conn_list[i].init();
		g_conn_list[i].next_free = i + 1;
	}
	// set last connect next_free is -1
	g_conn_list[MAX_CONNECT-1].next_free = -1;
	g_free_connect = 0;
	return 0;
}

unsigned long create_uid()
{
	static int64_t uid = 100;
	uid++;
	return uid;
}

http_connect_t * new_conn(int fd)
{
	if (g_free_connect == -1)
	{
		// no more free connect
		return NULL;
	}
	http_connect_t * pconn = g_conn_list + g_free_connect;
	pconn->fd = fd;
	pconn->uid = create_uid();
	pconn->active();
	g_free_connect = pconn->next_free;
	// add into g_uid_map
	g_uid_map.insert(std::make_pair(pconn->uid, pconn-g_conn_list));
	return pconn;
}

int free_conn(http_connect_t *pconn)
{
	if (pconn == NULL)
	{
		return -1;
	}
	g_uid_map.erase(pconn->uid);
	pconn->init();
	// replace the free connect
	pconn->next_free = g_free_connect;
	g_free_connect = pconn - g_conn_list;
	return 0;
}

// not real write, just add into send buffer and listen EPOLLOUT
int net_write(http_connect_t *pconn, std::string &buffer)
{
	if (pconn == NULL)
	{
		return -1;
	}

	auto_lock_t lock(&pconn->lock);
	if (pconn->is_valid() == 0)
	{
		return -1;
	}

	pconn->out_buffer.append(buffer);

	struct epoll_event c_ev;
	c_ev.data.ptr = (void*)pconn;
	c_ev.events = EPOLLIN | EPOLLOUT;
	epoll_ctl(g_epfd, EPOLL_CTL_MOD, pconn->fd, &c_ev);
	return 0;
}

///// CONNECTION UTIL END   ]

///// NET UTIL START [

void set_socket_nonblock(int fd)
{
	int opts;
	opts = fcntl(fd, F_GETFL);
	opts = opts | O_NONBLOCK;
	fcntl(fd, F_SETFL, opts);
}

int init_listen_fd(int epfd)
{

	// init listen_fd
	// 1.create normal socket
	// 2.set reuse, set non-block
	// 3.bind, listen
	// 4.add into epoll, event is EPOLL_IN 
	int ret;
	int listen_fd = -1;

	// 1. create socket
	struct sockaddr_in sin;
	bzero(&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(SERVER_PORT);
	int ip = inet_addr(SERVER_IP);
	sin.sin_addr.s_addr = ip;

	listen_fd = socket(sin.sin_family, SOCK_STREAM, 0);
	if (listen_fd < 0)
	{
		printf("init_listen_fd:socket fail errno=%d\n", errno);
		return listen_fd;
	}

	// 2.set reuse, non-block
	int flag;
	flag = 1;
	setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(int));
	set_socket_nonblock(listen_fd);

	// 3.bind, listen
	ret = bind(listen_fd, (struct sockaddr *)&sin, sizeof(sin));
	if (ret < 0)
	{
		printf("init_listen_fd:bind fail errno=%d\n", errno);
		return ret;
	}

	int backlog = 1024;
	ret = listen(listen_fd, backlog);
	if (ret < 0)
	{
		printf("init_listen_fd:listen fail errno=%d\n", errno);
		return ret;
	}

	/*
	typedef union epoll_data {
		void *ptr;
		int fd;
		__uint32_t u32;
		__uint64_t u64;
	} epoll_data_t;

	struct epoll_event {
		__uint32_t events;  // Epoll events
	    epoll_data_t data;  // User data variable
	};
	*/

	// 4.add into epoll
	struct epoll_event ev;
	ev.data.fd = listen_fd; // ev.data.fd and ev.data.ptr are union
	ev.events = EPOLLIN; 
	epoll_ctl(epfd, EPOLL_CTL_ADD, listen_fd, &ev);

	return listen_fd;
}

// accept until EAGAIN
int do_accept(int listen_fd, int epfd)
{
	int ret = 0;
	while (true)
	{
		// accept client
		// 1.accept
		// 2.set nonblock, tcp nodealy
		// 3.add into epoll, event is EPOLLIN | EPOLLET
		struct sockaddr_in c_sin;
		bzero(&c_sin, sizeof(c_sin));
		socklen_t sin_size = sizeof(c_sin);
		int client_fd = accept(listen_fd, (struct sockaddr*)&c_sin, &sin_size);	
		if (client_fd < 0)
		{
			if (errno == EINTR || errno == EAGAIN || EWOULDBLOCK)
			{
				// no more connection to accept
				ret = 0;
				break;
			}
			else
			{
				// accept error, something go wrong
				printf("do_accept:accept error listen_fd=%d epfd=%d errno=%d\n", listen_fd, epfd, errno);
				ret = -1;
				break;
			}
		}
		printf("do_accept:new connect %d\n", client_fd);

		http_connect_t *pconn = new_conn(client_fd);
		if (pconn == NULL)
		{
			// cannot handle more connect
			printf("do_accept:can not handle more connect\n");
			close(client_fd);
			continue;
		}

		set_socket_nonblock(client_fd);

		int flag = 1; // stop Nagle
		ret = setsockopt(client_fd, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(int));

		#ifdef __linux__
		flag = 1; // keep alive
		ret = setsockopt(client_fd, SOL_SOCKET, SO_KEEPALIVE, &flag, sizeof(int));
		flag = 30; // after 30 seconds of idle, start probes
		ret = setsockopt(client_fd, SOL_TCP, TCP_KEEPIDLE, &flag, sizeof(int));
		flag = 5; // 5 sec of probes interval time
		ret = setsockopt(client_fd, SOL_TCP, TCP_KEEPINTVL, &flag, sizeof(int));
		flag = 3; // max probes count
		ret = setsockopt(client_fd, SOL_TCP, TCP_KEEPCNT, &flag, sizeof(int));
		#endif
		
		struct epoll_event c_ev;
		c_ev.data.ptr = (void*)pconn;
		c_ev.events = EPOLLIN;
		epoll_ctl(epfd, EPOLL_CTL_ADD, client_fd, &c_ev);
	}
	return 0;
}

int do_recv(http_connect_t *pconn, int epfd)
{
	// 1.read fd
	// 2.if read size == 0, remove from epoll, close fd
	// 3.handle msg

	int ret = 0;
	if (pconn == NULL)
	{
		printf("do_recv:pconn==NULL\n");
		return -1;
	}

	auto_lock_t lock(&pconn->lock);
	if (pconn->is_valid() == 0)
	{
		printf("do_recv:pconn invalid\n");
		return -1;
	}

	int size = read(pconn->fd, pconn->in_end_pos , MAX_CONNECT_BUFFER - (pconn->in_end_pos - pconn->in_start_pos));
	if (size == 0)
	{
		// EOF, client disconnect
		epoll_ctl(epfd, EPOLL_CTL_DEL, pconn->fd, NULL);
		close(pconn->fd);
		free_conn(pconn);
		printf("do_recv:EOF, close socket\n");
		return -1;
	}

	if (size < 0)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
		{
			// read no more, normal
			printf("do_recv:EAGAIN\n");
			return 0;
		}

		// something go wrong
		printf("*************** read error, errno=%d ***********\n", errno);
		epoll_ctl(epfd, EPOLL_CTL_DEL, pconn->fd, NULL);
		close(pconn->fd);
		free_conn(pconn);
		printf("do_recv:EOF, close socket\n");
		return -1;
	}
	pconn->in_end_pos += size;
	printf("do_recv:read size=%d\n", size);
	pconn->active();

	// handle http request
	int total_size = pconn->in_end_pos - pconn->in_start_pos;
	std::string str(pconn->in_start_pos, pconn->in_end_pos - pconn->in_start_pos);
	printf("str=[%s]\n", str.c_str());
	http_request_t request;
	ret = request.unpack(pconn->in_start_pos, pconn->in_end_pos - pconn->in_start_pos);
	if (ret < 0)
	{
		// http format error, close this connect
		printf("*************** http format error ***********\n");
		epoll_ctl(epfd, EPOLL_CTL_DEL, pconn->fd, NULL);
		close(pconn->fd);
		free_conn(pconn);
		return -1;
	}
	if (ret == 0)
	{
		// in_buffer not include a full http request, return first
		return 0;
	}

	// TODO 
	// 1.should add request to a thread pool
	// 2.thread task handle request

	http_response_t response;
	response.status = http_response_t::RESPONSE_STATUS_CODE::STATUS_OK;
	response.content_type = http_response_t::RESPONSE_CONTENT_TYPE::TYPE_TEXT;
	response.content = get_simple_page_content();
	std::string response_buffer;
	response.pack(response_buffer);
	net_write(pconn, response_buffer);

	// now we get a full http request
	total_size -= ret;

	if (total_size == 0)
	{
		printf("do_recv:all buffer handled\n");
		pconn->in_start_pos = pconn->in_buffer;
		pconn->in_end_pos = pconn->in_buffer;
	}

	return 0;
}

int do_send(http_connect_t *pconn, int epfd)
{
	if (pconn == NULL) {
		printf("do_send:pconn==NULL\n");
		return -1;
	}

	auto_lock_t lock(&pconn->lock);
	if (pconn->is_valid() == 0)
	{
		printf("do_send:pconn invalid\n");
		return -1;
	}

	if (pconn->out_buffer.size() == 0)
	{
		// no more to send, mod as EPOLLIN
		struct epoll_event c_ev;
		c_ev.data.ptr = (void*)pconn;
		c_ev.events = EPOLLIN;
		epoll_ctl(epfd, EPOLL_CTL_MOD, pconn->fd, &c_ev);
		return 0;
	}

	int size = write(pconn->fd, pconn->out_buffer.c_str() + pconn->out_pos, pconn->out_buffer.size() - pconn->out_pos);
	if (size < 0)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
		{
			// kernel writer buff full, keep EPOLL_OUT event
			printf("do_send:write EAGAIN\n");
			return 0;
		}
		// some thing go wrong
		printf("**************** write error, errno=%d ****************\n", errno);
		return -1;
	}

	pconn->out_pos += size;
	if (pconn->out_buffer.size() == (size_t)pconn->out_pos)
	{
		// all out_buffer sended, now should only listen EPOLLIN
		pconn->out_buffer = "";
		pconn->out_pos = 0;
		struct epoll_event c_ev;
		c_ev.data.ptr = (void*)pconn;
		c_ev.events = EPOLLIN;
		epoll_ctl(epfd, EPOLL_CTL_MOD, pconn->fd, &c_ev);
	}
	return 0;
}

///// NET UTIL END ]

int main()
{
	int ret;
	printf("SERVER_IP=[%s] SERVER_PORT=[%d]\n", SERVER_IP, SERVER_PORT);

	pthread_mutex_init(&g_lock, NULL);

	// 1.init all connection
	// 2.init epoll
	// 3.init listen_fd
	// 4.epoll wait

	// 1.init connection
	init_connection_list();

	// 2.init epoll
	// must do epoll_create first
	// int epoll_create(int size); // size is unused nowday
	g_epfd = epoll_create(1);
	if (g_epfd < 0)
	{
		printf("epoll_create fail %d\n", errno);
		return -1;
	}

	// 3.init listen socket
	int listen_fd = init_listen_fd(g_epfd);
	if (listen_fd < 0)
	{
		return -1;
	}
	printf("main:listen_fd=%d\n", listen_fd);

	// 4.

	// max handle events in one loop
	const int MAX_EVENTS = 20;
	struct epoll_event events[MAX_EVENTS];	
	while (true)
	{
		// int epoll_wait(int epfd, struct epoll_event * events, int maxevents, int timeout);
		int nfds = epoll_wait(g_epfd, events, MAX_EVENTS, 5000);
		printf("nfds = %d\n", nfds);
		for (int i=0;i<nfds;i++)
		{
			// handle listen fd
			if (events[i].data.fd == listen_fd)
			{
				ret = do_accept(listen_fd, g_epfd);
				continue;
			}

			// read client
			if (events[i].events & EPOLLIN)
			{
				http_connect_t * pconn;
				pconn = (http_connect_t *)events[i].data.ptr;

				ret = do_recv(pconn, g_epfd);
				if (ret != 0)
				{
					continue;
				}
			}

			// send client
			if (events[i].events & EPOLLOUT)
			{
				http_connect_t * pconn;
				pconn = (http_connect_t *)events[i].data.ptr;
				ret = do_send(pconn, g_epfd);
			}
		}

		// disconnect timeout connect
		time_t now = time(NULL);
		for (int i = 0; i < MAX_CONNECT; i++)
		{
			http_connect_t *pconn = g_conn_list + i;
			if (pconn->is_valid() == 0)
			{
				continue;
			}
			if (now - pconn->active_time <= CONNECT_TIMEOUT)
			{
				continue;
			}
			printf("close timeout connect[%d]\n", i);
			epoll_ctl(g_epfd, EPOLL_CTL_DEL, pconn->fd, NULL);
			close(pconn->fd);
			free_conn(pconn);
		}
	}
	
	return 0;
}
