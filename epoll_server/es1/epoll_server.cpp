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


#define SERVER_PORT 7711
#define SERVER_IP "0.0.0.0"

#define MAX_CONNECT	10

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
	// 1.create normal socket, bind, listen
	// 2.add into epoll, event is EPOLL_IN | EPOLL_ET
	
	struct sockaddr_in sin;
	bzero(&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(SERVER_PORT);
	int ip = inet_addr(SERVER_IP);
	sin.sin_addr.s_addr = ip;

	int listen_fd = socket(sin.sin_family, SOCK_STREAM, 0);
	if (listen_fd < 0)
	{
		printf("init_listen_fd:socket fail\n");
		return listen_fd;
	}

	int flag;
	flag = 1;
	setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(int));
	set_socket_nonblock(listen_fd);

	ret = bind(listen_fd, (struct sockaddr *)&sin, sizeof(sin));
	if (ret < 0)
	{
		printf("init_listen_fd:bind fail\n");
		return ret;
	}

	int backlog = 1024;
	ret = listen(listen_fd, backlog);
	if (ret < 0)
	{
		printf("init_listen_fd:listen fail\n");
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

	struct epoll_event ev;
	ev.data.fd = listen_fd; // ev.data.fd and ev.data.ptr are union
	ev.events = EPOLLIN | EPOLLET; // EPOLLET: edge triggered
	// ev.events = EPOLLIN;
	// int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
	epoll_ctl(epfd, EPOLL_CTL_ADD, listen_fd, &ev);

	return listen_fd;
}

///// NET UTIL END   ]

///// CONNECTION UTIL START [

struct connect_t{
	int fd;
	int next_free;
	std::string out_buffer;
	void Init()
	{
		fd = 0;
		next_free = 0;
		out_buffer = "";
	}
};

int g_free_connect = 0;
connect_t g_conn_list[MAX_CONNECT];

int init_connection_list() {
	for (int i=0;i<MAX_CONNECT;i++) {
		g_conn_list[i].Init();
		g_conn_list[i].next_free = i + 1;
	}
	g_conn_list[MAX_CONNECT-1].next_free = -1;
	g_free_connect = 0;
	return 0;
}

connect_t * new_conn(int fd) {
	connect_t * conn = &g_conn_list[g_free_connect];
	conn->fd = fd;
	g_free_connect = conn->next_free;
	return conn;
}

int free_conn(connect_t * conn) {
	conn->Init();
	conn->next_free = g_free_connect;
	g_free_connect = conn - g_conn_list;
	return 0;
}

///// CONNECTION UTIL END   ]

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
	// 4.add listen_fd into epoll
	// 5.epoll wait

	init_connection_list();

	struct epoll_event events[20];	

	// must do epoll_create first
	// int epoll_create(int size); // size is unused nowday
	int epfd = 0;
	epfd = epoll_create(1);

	int listen_fd = init_listen_fd(epfd);
	if (listen_fd < 0)
	{
		return -1;
	}
	printf("listen_fd=%d\n", listen_fd);

	printf("EPOLLIN=%d EPOLLOUT=%d\n", EPOLLIN, EPOLLOUT);

	while (true)
	{
		// int epoll_wait(int epfd, struct epoll_event * events, int maxevents, int timeout);
		int nfds = epoll_wait(epfd, events, 20, 10000);
		printf("nfds = %d\n", nfds);
		for (int i=0;i<nfds;i++)
		{
			printf("events[i].events=%d\n", events[i].events);

			if (events[i].data.fd == listen_fd)
			{
				printf("\n---- ACCEPT START\n");
				// listen_fd set EPOLL_ET, accept until EAGAIN
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
							break;
						}
						else
						{
							// accept error, some thing to wrong
							printf("accept error, errno=%d\n", errno);
							break;
						}
					}
					printf("accept:new_conn %d\n", client_fd);

					set_socket_nonblock(client_fd);

					int flag = 1;
					ret = setsockopt(client_fd, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(int));
					
					struct epoll_event c_ev;
					connect_t * conn = new_conn(client_fd);
					c_ev.data.ptr = (void*)conn;
					c_ev.events=EPOLLIN | EPOLLET;
					epoll_ctl(epfd, EPOLL_CTL_ADD, client_fd, &c_ev);
				}

				printf("---- ACCEPT END\n\n");
				continue;
			}

			// read client
			if (events[i].events & EPOLLIN)
			{
				// 1.read fd, read until EAGAIN
				// 2.if read size == 0, remove from epoll, close fd
				// 3.do send, mod EPOLLOUT in epoll
				printf("\n---- READ START\n");
				const int BUFFER_SIZE = 1024000;
				char in_buffer[BUFFER_SIZE+1];
				bzero(in_buffer, BUFFER_SIZE+1);

				int conn_id = 0;
				connect_t * conn;
				conn = (connect_t *)events[i].data.ptr;
				if (conn == NULL) {
					printf("conn==NULL\n");
					continue;
				}
				conn_id = conn-g_conn_list;
				if (conn->fd <= 0) {
					printf("conn->fd<=0 %d\n", conn->fd);
					continue;
				}

				bool isClientClose = false;
				while (true)
				{
					int size = read(conn->fd, in_buffer, BUFFER_SIZE);
					if (size == 0)
					{
						// EOF, client disconnect
						epoll_ctl(epfd, EPOLL_CTL_DEL, conn->fd, &events[i]);
						close(conn->fd);
						free_conn(conn);
						isClientClose = true;
						printf("read EOF, close socket\n");
						break;
					}

					if (size < 0)
					{
						if (errno == EAGAIN || errno == EWOULDBLOCK)
						{
							// read no more
							printf("read EAGAIN\n");
							break;
						}
						// something go wrong
						printf("*************** read error, errno=%d ***********\n", errno);
						break;
					}

					in_buffer[size] = '\0';
					// printf("read:fd=%d conn_id=%d in_buffer=%s\n", conn->fd, conn_id, in_buffer);
					printf("read size=%d\n", size);
					// send back to client
					conn->out_buffer += in_buffer;
				}

				if (isClientClose == true)
				{
					continue;
				}

				struct epoll_event c_ev;
				c_ev.data.ptr = (void*)conn;
				// c_ev.events = EPOLLOUT | EPOLLET;
				c_ev.events = EPOLLIN | EPOLLOUT | EPOLLET;
				epoll_ctl(epfd, EPOLL_CTL_MOD, conn->fd, &c_ev);

				printf("---- READ END\n\n");
			}

			// send client
			if (events[i].events & EPOLLOUT)
			{
				printf("\n---- WRITE START\n");
				int conn_id = 0;
				connect_t * conn;
				conn = (connect_t *)events[i].data.ptr;
				if (conn == NULL) {
					printf("conn==NULL\n");
					continue;
				}
				conn_id = conn-g_conn_list;
				printf("write:fd=%d conn_id=%d\n", conn->fd, conn_id);
				
				if (conn->fd <= 0) {
					printf("conn->fd<=0 %d\n", conn->fd);
					continue;
				}

				printf("before write:conn->out_buffer.size()=%lu\n", conn->out_buffer.size());

				if (conn->out_buffer.size() == 0)
				{
					// no more to send, mod as EPOLLIN
					struct epoll_event c_ev;
					c_ev.data.ptr = (void*)conn;
					c_ev.events = EPOLLIN | EPOLLET;
					epoll_ctl(epfd, EPOLL_CTL_MOD, conn->fd, &c_ev);
					continue;
				}

				while (true)
				{
					int size = write(conn->fd, conn->out_buffer.c_str(), conn->out_buffer.size());
					if (size < 0)
					{
						if (errno == EAGAIN || errno == EWOULDBLOCK)
						{
							// kernel writer buff full, keep EPOLL_OUT event
							printf("write EAGAIN\n");
							break;
						}
						// some thing go wrong
						printf("**************** write error, errno=%d ****************\n", errno);
						break;
					}
					conn->out_buffer = conn->out_buffer.substr(size);
					if (conn->out_buffer.size() == 0)
					{
						break;
					}
				}

				printf("after write:conn->out_buffer.size()=%lu\n", conn->out_buffer.size());

				if (conn->out_buffer.size() == 0)
				{
					// all out_buffer sended, now should only listen EPOLLIN
					struct epoll_event c_ev;
					c_ev.data.ptr = (void*)conn;
					c_ev.events = EPOLLIN | EPOLLET;
					epoll_ctl(epfd, EPOLL_CTL_MOD, conn->fd, &c_ev);
				}

				printf("---- WRITE END\n\n");
			}
		}
	}
	
	return 0;
		
}
