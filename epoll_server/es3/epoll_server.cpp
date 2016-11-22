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

#include <string>
#include <fstream>
#include <map>

#include "msg_head.h"


#define SERVER_PORT 7711
#define SERVER_IP "0.0.0.0"

#define MAX_CONNECT	1
#define IN_BUFFER_SIZE 1024000

int g_epfd = -1;

///// UTIL START [
static int trim(char *str, int max)
{
	if (max <= 0) {
		max = strlen(str);
	}

	int i;
	for (i=max-1;i>=0;i--) {;
		if (str[i]=='\r' || str[i]=='\n' || str[i]==' ') {
			str[i] = '\0';
			max --;
		} else {
			return max;
		}
	}
	return max;
}

static void read_file(const char *filename, std::string &buffer)
{
	std::ifstream ifile(filename, std::ios::in);	
	if (ifile.is_open() && ifile.good() && !ifile.eof())
	{
		// 1.get total size
		// 2.new same size tmp buffer
		// 3.move seekg to begin
		// 4.read into buffer
		// 5.copy tmp buffer to string buffer
		// 6.delete tmp buffer

		long startPos = ifile.tellg();
		ifile.seekg(0, std::ios::end);
		long endPos = ifile.tellg();

		long len = endPos - startPos;
		char *tmpBuffer = new char[len];
		ifile.seekg(0, std::ios::beg);
		ifile.read(tmpBuffer, len);

		buffer = tmpBuffer;

		delete tmpBuffer;
	}
	ifile.close();
}
///// UTIL END   ]

///// CONNECTION UTIL START [

struct connect_t
{
	int fd;
	unsigned long uid;
	int next_free;

	char in_buffer[IN_BUFFER_SIZE+1];
	char *in_start_pos;
	char *in_end_pos;
	std::string out_buffer;
	int out_pos;

	void init()
	{
		fd = 0;
		uid = 0;
		next_free = 0;
		in_start_pos = in_buffer;
		in_end_pos = in_buffer;
		out_buffer = "";
		out_pos = 0;
	}
};

int g_free_connect = 0;  // the free connect id
connect_t g_conn_list[MAX_CONNECT];
std::map<unsigned long, int> g_uid_map; // uid to connect id

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
	static unsigned long uid = 100;
	uid++;
	return uid;
}

connect_t * new_conn(int fd)
{
	if (g_free_connect == -1)
	{
		return NULL;
	}
	connect_t * conn = &g_conn_list[g_free_connect];
	conn->fd = fd;
	conn->uid = create_uid();
	g_free_connect = conn->next_free;
	printf("g_free_connect=%d\n", g_free_connect);
	return conn;
}

int free_conn(connect_t * conn)
{
	conn->init();
	conn->next_free = g_free_connect;
	g_free_connect = conn - g_conn_list;
	return 0;
}

int net_write(connect_t *conn, msg_head_t *header, const char *body, int size)
{
	char header_data[100];
	header->body_len = size;
	header->pack(header_data);
	conn->out_buffer.append(header_data, msg_head_t::MSG_HEAD_LEN);
	conn->out_buffer.append(body, header->body_len);

	struct epoll_event c_ev;
	c_ev.data.ptr = (void*)conn;
	c_ev.events = EPOLLIN | EPOLLOUT;
	epoll_ctl(g_epfd, EPOLL_CTL_MOD, conn->fd, &c_ev);
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
	int ret;

	// init listen_fd
	// 1.create normal socket
	// 2.set reuse, set non-block
	// 3.bind, listen
	// 2.add into epoll, event is EPOLL_IN 
	
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
			if (errno == EAGAIN || EWOULDBLOCK)
			{
				// no more connection
				ret = 0;
				break;
			}
			else
			{
				// accept error, some thing to wrong
				printf("do_accept:accept error errno=%d\n", errno);
				ret = -1;
				break;
			}
		}
		printf("do_accept:new connect %d\n", client_fd);

		connect_t * conn = new_conn(client_fd);
		if (conn == NULL)
		{
			// cannot handle more connect
			printf("do_accept:can not handle more connect\n");
			close(client_fd);
			continue;
		}

		set_socket_nonblock(client_fd);

		int flag = 1;
		ret = setsockopt(client_fd, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(int));
		
		struct epoll_event c_ev;
		c_ev.data.ptr = (void*)conn;
		c_ev.events = EPOLLIN;
		epoll_ctl(epfd, EPOLL_CTL_ADD, client_fd, &c_ev);
	}
	return 0;
}

int read_msg(char *in_buffer, int size, msg_head_t *header, char *body)
{
	if (size < msg_head_t::MSG_HEAD_LEN)
	{
		return 0;
	}
	char *ptr = header->unpack(in_buffer);
	if (size - msg_head_t::MSG_HEAD_LEN < header->body_len)
	{
		return 0;
	}

	memcpy(body, ptr, header->body_len);
	
	return msg_head_t::MSG_HEAD_LEN + header->body_len;
}

int do_read(connect_t *conn, int epfd)
{
	// 1.read fd
	// 2.if read size == 0, remove from epoll, close fd
	// 3.handle msg

	int ret = 0;
	if (conn == NULL)
	{
		printf("do_read:conn==NULL\n");
		return -1;
	}
	if (conn->fd <= 0)
	{
		printf("do_read:conn->fd<=0 %d\n", conn->fd);
		return -1;
	}

	int size = read(conn->fd, conn->in_end_pos , IN_BUFFER_SIZE - (conn->in_end_pos - conn->in_start_pos));
	if (size == 0)
	{
		// EOF, client disconnect
		epoll_ctl(epfd, EPOLL_CTL_DEL, conn->fd, NULL);
		close(conn->fd);
		free_conn(conn);
		printf("do_read:EOF, close socket\n");
		return -1;
	}

	if (size < 0)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
		{
			// read no more, normal
			printf("do_read:EAGAIN\n");
			return 0;
		}

		// something go wrong
		printf("*************** read error, errno=%d ***********\n", errno);
		epoll_ctl(epfd, EPOLL_CTL_DEL, conn->fd, NULL);
		close(conn->fd);
		free_conn(conn);
		printf("do_read:EOF, close socket\n");
		return -1;
	}
	conn->in_end_pos += size;
	printf("do_read:read size=%d\n", size);


	// handle msg

	msg_head_t header;
	char body[IN_BUFFER_SIZE+1];

	int total_size = conn->in_end_pos - conn->in_start_pos;
	while (conn->in_end_pos != conn->in_start_pos)
	{
		ret = read_msg(conn->in_start_pos, total_size, &header, body);
		if (ret == 0)
		{
			// no more msg
			break;
		}

		printf("do_read:cmd_id=%d body_len=%d msg_index=%u\n", header.cmd_id, header.body_len, header.msg_index);
		body[header.body_len] = '\0';
		printf("do_read:body=[%s]\n", body);

		{
			// send data to client
			char body_data[1024];
			int size = sprintf(body_data, "%lu", time(NULL));
			net_write(conn, &header, body_data, size);
		}
		
		conn->in_start_pos += ret;
		total_size -= ret;
	}

	if (total_size == 0)
	{
		printf("do_read:all buffer handled\n");
		conn->in_start_pos = conn->in_buffer;
		conn->in_end_pos = conn->in_buffer;
	}

	return 0;
}

int do_write(connect_t *conn, int epfd)
{
	if (conn == NULL) {
		printf("do_write:conn==NULL\n");
		return -1;
	}
	
	if (conn->fd <= 0) {
		printf("do_write:conn->fd<=0 %d\n", conn->fd);
		return -1;
	}

	if (conn->out_buffer.size() == 0)
	{
		// no more to send, mod as EPOLLIN
		struct epoll_event c_ev;
		c_ev.data.ptr = (void*)conn;
		c_ev.events = EPOLLIN;
		epoll_ctl(epfd, EPOLL_CTL_MOD, conn->fd, &c_ev);
		return 0;
	}

	int size = write(conn->fd, conn->out_buffer.c_str() + conn->out_pos, conn->out_buffer.size() - conn->out_pos);
	if (size < 0)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
		{
			// kernel writer buff full, keep EPOLL_OUT event
			printf("do_write:write EAGAIN\n");
			return 0;
		}
		// some thing go wrong
		printf("**************** write error, errno=%d ****************\n", errno);
		return -1;
	}

	conn->out_pos += size;
	if (conn->out_buffer.size() == (size_t)conn->out_pos)
	{
		// all out_buffer sended, now should only listen EPOLLIN
		conn->out_buffer = "";
		conn->out_pos = 0;
		struct epoll_event c_ev;
		c_ev.data.ptr = (void*)conn;
		c_ev.events = EPOLLIN;
		epoll_ctl(epfd, EPOLL_CTL_MOD, conn->fd, &c_ev);
	}
	return 0;
}

///// NET UTIL END ]

int main()
{
	int ret;

	/*
	const char * filename = "./tmp.txt";
	std::string sendData;
	read_file(filename, sendData); // about 80k
	sendData += sendData; // double it
	sendData += sendData; // double it
	sendData += sendData; // double it
	sendData += sendData; // double it
	sendData += sendData; // double it
	sendData += sendData; // double it
	printf("sendData.size()=%lu\n", sendData.size());
	*/

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
		int nfds = epoll_wait(g_epfd, events, MAX_EVENTS, 10000);
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
				connect_t * conn;
				conn = (connect_t *)events[i].data.ptr;

				ret = do_read(conn, g_epfd);
				if (ret != 0)
				{
					continue;
				}
			}

			// send client
			if (events[i].events & EPOLLOUT)
			{
				connect_t * conn;
				conn = (connect_t *)events[i].data.ptr;
				ret = do_write(conn, g_epfd);
			}
		}
	}
	
	return 0;
		
}
