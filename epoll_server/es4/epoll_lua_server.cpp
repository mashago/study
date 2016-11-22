
#if __APPLE__
// @see http://www.opensource.apple.com/source/mDNSResponder/mDNSResponder-258.18/mDNSPosix/PosixDaemon.c
#define daemon avoid_warning_thankyou
#endif

extern "C"
{
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

#include <sys/mman.h>
#include <sys/types.h>

#if __APPLE__
#undef daemon
extern int daemon(int, int);
#endif

#include "socket_poll.h"
}

#include <string>
#include <fstream>


#define SERVER_PORT 7711
#define SERVER_IP "0.0.0.0"

#define MAX_CONNECT	10
#define MAX_IN_BUFFER 1024

bool is_running = true;

///// CONNECTION UTIL START [

struct connect_t{
	int fd_;
	int next_free_;
	
	char in_buffer_[MAX_IN_BUFFER];
	char *in_start_pos_;
	char *in_end_pos_;

	std::string out_buffer_;
	int out_pos_;

	void init()
	{
		fd_ = -1;
		next_free_ = 0;
		in_start_pos_ = in_buffer_;
		in_end_pos_ = in_buffer_;
		out_buffer_ = "";
		out_pos_ = 0;
	}
};

int g_pfd = -1;
connect_t g_listen_conn;
int g_free_connect = 0;
connect_t g_conn_list[MAX_CONNECT];

int init_connection_list() {
	for (int i=0;i<MAX_CONNECT;i++) {
		g_conn_list[i].init();
		g_conn_list[i].next_free_ = i + 1;
	}
	g_conn_list[MAX_CONNECT-1].next_free_ = -1;
	g_free_connect = 0;
	return 0;
}

connect_t * new_conn(int fd) {
	connect_t * conn = &g_conn_list[g_free_connect];
	conn->fd_ = fd;
	g_free_connect = conn->next_free_;
	return conn;
}

int free_conn(connect_t * conn) {
	conn->init();
	conn->next_free_ = g_free_connect;
	g_free_connect = conn - g_conn_list;
	return 0;
}

///// CONNECTION UTIL END   ]

///// NET UTIL START [

int init_listen_fd(const char *ip, int port)
{
	int ret = 0;

	// init listen_fd
	// 1.create normal socket
	// 2.set reuse, set non-block
	// 3.bind, listen
	
	int listen_fd = -1;
	
	// 1. create socket
	struct sockaddr_in sin;
	bzero(&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	int ip_num = inet_addr(ip);
	sin.sin_addr.s_addr = ip_num;

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
	sp_nonblocking(listen_fd);

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

	g_listen_conn.init();
	g_listen_conn.fd_ = listen_fd;

	return listen_fd;
}

int do_accept(int listen_fd, int pfd)
{
	int max_accept_count = 10;
	int ret = 0;

	while (max_accept_count--)
	{
		// accept client
		// 1.accept
		// 2.set nonblock, tcp nodealy
		// 3.add into poll
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
				// accept error, something go wrong
				printf("************* do_accept:accept error %d **************\n", errno);
				ret = -1;
				break;
			}
		}

		connect_t * conn = new_conn(client_fd);
		if (conn == NULL)
		{
			// cannot handle more connect
			printf("************* do_accept:can not handle more connect ************\n");
			close(client_fd);
			continue;
		}

		sp_nonblocking(client_fd);

		// int flag = 1;
		// ret = setsockopt(client_fd, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(int));
		
		sp_add(pfd, client_fd, (void *)conn);

	}
	return ret;
}

void msg_send(int pfd, connect_t *conn, const char *buffer, int size)
{
	conn->out_buffer_.append(buffer, size);
	sp_write(pfd, conn->fd_, (void *)conn, true);
}

int do_read(connect_t *conn, int pfd)
{
	// 1.read fd
	// 2.if read size == 0, remove from poll, close fd
	// 3.handle msg

	// int ret = 0;
	if (conn == NULL)
	{
		printf("do_read:conn==NULL\n");
		return -1;
	}
	if (conn->fd_ <= 0)
	{
		printf("do_read:conn invalid %ld\n", conn - g_conn_list);
		return -1;
	}

	int size = read(conn->fd_, conn->in_end_pos_ , MAX_IN_BUFFER - (conn->in_end_pos_ - conn->in_buffer_));
	if (size == 0)
	{
		// EOF, server disconnect
		printf("******** do_read:server shutdown conn->fd_=%d\n", conn->fd_);
		sp_del(pfd, conn->fd_);
		free_conn(conn);
		return -1;
	}

	if (size < 0)
	{
		if (errno == EINTR)
		{
			return 0;
		}

		if (errno == EAGAIN || errno == EWOULDBLOCK)
		{
			// read no more, normal?
			printf("do_read:EAGAIN\n");
			return 0;
		}

		// something go wrong
		printf("*************** do_read:read error, errno=%d ***********\n", errno);
		sp_del(pfd, conn->fd_);
		free_conn(conn);
		return -1;
	}
	conn->in_end_pos_ += size;

	// debug print
	// *(conn->in_end_pos_) = '\0';
	// printf("conn read: [%s]\n", conn->in_start_pos_);
	
	msg_send(pfd, conn, conn->in_start_pos_, conn->in_end_pos_ - conn->in_start_pos_);
	conn->in_end_pos_ = conn->in_buffer_;

	return 0;
}

int do_write(connect_t *conn, int pfd)
{
	if (conn == NULL)
	{
		printf("do_write:conn==NULL");
		return -1;
	}
	
	if (conn->fd_ <= 0)
	{
		printf("do_write:conn invalid %ld\n", conn - g_conn_list);
		return -1;
	}

	if (conn->out_buffer_.size() == 0)
	{
		// no more to send, cancel listen write
		sp_write(pfd, conn->fd_, (void *)conn, false);
		return 0;
	}

	int size = write(conn->fd_, conn->out_buffer_.c_str() + conn->out_pos_, conn->out_buffer_.size() - conn->out_pos_);
	if (size < 0)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
		{
			// kernel writer buff full, keep EPOLL_OUT event
			printf("do_write:EAGAIN");
			return 0;
		}
		// XXX some thing go wrong, should we delete connect?
		printf("**************** write error, errno=%d ****************", errno);
		return -1;
	}

	conn->out_pos_ += size;
	// check if all out_buffer_ sended
	if (conn->out_buffer_.size() == (size_t)conn->out_pos_)
	{
		// 1.clean out_buffer_
		// 2.cancel listen write
		conn->out_buffer_ = "";
		conn->out_pos_ = 0;

		sp_write(pfd, conn->fd_, (void *)conn, false);
	}
	return 0;
}

///// NET UTIL END   ]


int do_daemon(bool is_daemon)
{
	if (!is_daemon)
	{
		return 0;
	}
	return daemon(1, 1);

	/*
	pid_t pid;
	pid = fork();
	if (pid < 0)
	{
		return -1;
	}
	else if (pid)
	{
		_exit(0);
	}

	if (setsid() < 0)
	{
		return -1;
	}
	return 0;
	*/
}

int detach_process(int pfd)
{
	int ret = 0;
	// event num every loop handle
	const int MAX_EVENTS = MAX_CONNECT;
	event events[MAX_EVENTS];

	while (is_running)
	{
		// 1.sp_wait
		// 2.do accept, read, write

		// 1.
		int nfds = sp_wait(pfd, events, MAX_EVENTS, -1);
		if (nfds == -1)
		{
			if (errno == EINTR)
			{
				continue;
			}
			printf("sp_wait error %d\n", errno);
			return -1;
		}

		// 2.
		for (int i = 0; i < nfds; i++)
		{
			connect_t * conn;
			conn = (connect_t *)events[i].s;

			// listen_fd
			if (conn == &g_listen_conn)
			{
				ret = do_accept(conn->fd_, pfd);
				continue;
			}

			// client can read
			if (events[i].read)
			{
				do_read(conn, pfd);
			}

			// client can send
			if (events[i].write)
			{
				do_write(conn, pfd);
			}
		}

		// one loop end
	}

	return 0;
}

void do_clean()
{
	sp_release(g_pfd);
}

void signal_handler(int sig)
{
	printf("signal get [%d]\n", sig);
	switch (sig)
	{
	case SIGSEGV :
		printf("singal_handler:g_free_connect=%d\n", g_free_connect);
		break;
	case SIGINT :
	case SIGTERM :
		is_running = false; // if false, server will shutdown
		printf("server will shutdown\n");
		break;
	}
}

int main(int argc, char **argv)
{

	// 0.do daemon
	// 2.init all connection
	// 3.init listen_fd
	// 4.init poll
	// 5.listen accept
	// 6.loop poll wait
	// 7.server down, do clean
	
	bool is_daemon = false;

	// -d for daemon
	int opt;
	while ((opt = getopt(argc, argv, "d")) != -1)
	{
		switch (opt)
		{
		case 'd' :
			is_daemon = true;
			break;
		default :
			fprintf(stderr, "Usage: %s [-d]\n", argv[0]);
			exit(1);
		}
	}
	printf("is_daemon=%d\n", is_daemon);

	if (do_daemon(is_daemon) < 0)
	{
		printf("********** do_daemon fail\n");
		return -1;
	}
	
	signal(SIGINT, signal_handler);
	signal(SIGTERM, signal_handler);
	signal(SIGSEGV, signal_handler);

	// 2.init connection
	init_connection_list();

	printf("SERVER_IP=%s SERVER_PORT=%d\n", SERVER_IP, SERVER_PORT);
	// 3.init listen socket
	int listen_fd = init_listen_fd(SERVER_IP, SERVER_PORT);
	if (listen_fd < 0)
	{
		printf("********** init_listen_fd fail **********\n");
		return -1;
	}
	printf("main:listen_fd=%d\n", listen_fd);

	// 4.init poll
	g_pfd = sp_create();
	if (sp_invalid(g_pfd))
	{
		printf("********** sp_create fail %d **********\n", errno);
		return -1;
	}

	// 5
	sp_add(g_pfd, listen_fd, (void *)&g_listen_conn);

	// 6.loop and wait
	detach_process(g_pfd);

	// 7.server down, do clean 
	do_clean();
	
	printf("---- SERVER DOWN ----\n");

	return 0;
		
}
