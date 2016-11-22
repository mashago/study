
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>

#include "fdpoll.h"
#include "net_util.h"

#define MAX_CONNECT		10
#define SERVER_IP		("0.0.0.0")
#define SERVER_PORT		7777
#define FDPOLL_TIMEOUT	10000 // msec

#define SOCKET_NONBLOCK		1

#define STATE_READ			1
#define STATE_WRITE			2

#define BUFFER_SIZE			2048

typedef struct {
	int fd;
	int state;
	int next_free;
	int out_offset;
	int out_len;
	char out_buffer[BUFFER_SIZE];
} connect_t;
int g_free_connect;
connect_t g_connect_list[MAX_CONNECT];

///////////////// FUNCTION DECLARATION START //////// [
void init_connect_list();
connect_t * get_conn(int idx);
connect_t * new_conn(int fd);
void free_conn(connect_t *conn);
void do_disconnect(connect_t *conn);

int do_read(connect_t *conn);
int do_write(connect_t *conn);

int init_server();
int handle_newconnect(int listen_fd);

void sig_chld(int signo);
void sig_pipe(int signo);
///////////////// FUNCTION DECLARATION END ////////// ]

///////////////// CONNECT FUNCTION START //////////// [
void init_connect_list()
{
	bzero(g_connect_list, sizeof(g_connect_list));
	for (int i=0; i<MAX_CONNECT; i++) {
		g_connect_list[i].next_free = i+1;
	}
	g_connect_list[MAX_CONNECT-1].next_free = -1;
	g_free_connect = 0;
}

connect_t * get_conn(int idx)
{
	if (idx < 0 || idx >= MAX_CONNECT) {
		return NULL;
	}
	return g_connect_list + idx;
}

connect_t * new_conn(int fd)
{
	connect_t *conn = get_conn(g_free_connect);
	if (conn == NULL) {
		return NULL;
	}

	g_free_connect = conn->next_free;
	conn->next_free = -1;
	conn->fd = fd;
	conn->state = STATE_READ;
	conn->out_offset = 0;
	conn->out_len = 0;
	conn->out_buffer[0] = '\0';
	return conn;
}

void free_conn(connect_t *conn)
{
	bzero(conn, sizeof(connect_t));
	conn->next_free = g_free_connect;
	g_free_connect = conn - g_connect_list;
}

void do_disconnect(connect_t *conn)
{
	fdpoll_del(conn->fd);
	close(conn->fd);
	free_conn(conn);
}
///////////////// CONNECT FUNCTION END //////////// ]

///////////////// CLIENT FUNCTION START /////////// [
int net_write(connect_t *conn, const char *buffer)
{
	if (conn == NULL) {
		return -1;
	}
	int size = strlen(buffer);
	if (conn->out_len + size + 2 > BUFFER_SIZE) {
		do_disconnect(conn);
	}

	size = sprintf(conn->out_buffer+conn->out_len, "%s", buffer);
	conn->out_len += size;
	conn->state = STATE_WRITE;
	fdpoll_del(conn->fd);
	fdpoll_add(conn->fd, conn, FD_WRITE);

	return 0;
}

int do_read(connect_t *conn)
{
	int size;
	char buffer[BUFFER_SIZE + 1];

	size = read(conn->fd, buffer, BUFFER_SIZE-1);
	// EOF, do disconnect 
	if (size == 0) {
		printf("do_read:eof\n");
		do_disconnect(conn);	
		return 0;
	}
	if (size < 0) {
		printf("do_read:errno=%d\n", errno);
		if (errno == EINTR || errno == EAGAIN) {
			errno = 0;
			return 0;
		}
		do_disconnect(conn);
		return 0;
	}

	buffer[size] = '\0';

	printf("do_read:buffer=[%s]\n", buffer);

	net_write(conn, buffer);

	conn->state = STATE_WRITE;
	fdpoll_del(conn->fd);
	fdpoll_add(conn->fd, conn, FD_WRITE);

	return 0;
}

int do_write(connect_t *conn)
{
	int size;
	size = write(conn->fd, conn->out_buffer+conn->out_offset, conn->out_len-conn->out_offset);
	if (size < 0) {
		printf("do_write:errno=%d\n", errno);
		if (errno == EINTR || errno == EAGAIN) {
			errno = 0;
			return 0;
		}
		if (errno == EPIPE) {
			printf("do_write:EPIPE happend\n");
		}
		do_disconnect(conn);	
		return 0;
	}

	conn->out_offset += size;
	if (conn->out_offset >= conn->out_len) {
		conn->out_offset = 0;
		conn->out_len = 0;
		conn->out_buffer[0] = '\0';

		conn->state = STATE_READ;
		fdpoll_del(conn->fd);
		fdpoll_add(conn->fd, conn, FD_READ);
	}

	return 0;
}
///////////////// CLIENT FUNCTION END ///////////// ]

///////////////// SOCKET FUNCTION START /////////// [
// return listen_fd
int init_server()
{
	int listen_fd = INIT_SERVER_SOCKET(SERVER_IP, SERVER_PORT, SOCKET_NONBLOCK);
	if (listen_fd < 0) {
		return -6;
	}
	// printf("listen_fd=%d\n", listen_fd);
	connect_t * conn = new_conn(listen_fd);
	if (conn == NULL) {
		return -6;
	}
	fdpoll_add(listen_fd, (void*)conn, FD_READ);

	return listen_fd;
}

int handle_newconnect(int listen_fd)
{
	int ret;
	struct sockaddr_in cin;
	socklen_t len = sizeof(struct sockaddr_in);
	do {
		// too much connect
		if (g_free_connect == -1) {
			int fd = accept(listen_fd, (struct sockaddr*) &cin, &len);
			if (fd > 0) {
				const char * errstr = "sys -1 too many connect\n";
				write(fd, errstr, strlen(errstr));
				close(fd);
				return 0;
			}
		}

		// handle new connect
		int fd = accept(listen_fd, (struct sockaddr*) &cin, &len);
		if (fd < 0) {
			if (errno==EAGAIN) {
				// non-blocking io accept no more, return
				return 0;
			} else if (errno == EINTR) {
				// interrupted by single, do accept again
				continue;
			}
			// other accept error, return	
			printf("handle_accept:accept %d\n", errno);
			return -1;
		}
		printf("handle_newconnect:fd=%d\n", fd);
	
		ret = INIT_CLIENT_SOCKET(fd, SOCKET_NONBLOCK);
		if (ret < 0) {
			printf("handle_newconnect:init_client_socket %d\n", fd);
		}

		connect_t * conn = new_conn(fd);
		if (conn == NULL) {
			printf("handle_newconnect:new_conn_null\n");
			return -1;
		}
		fdpoll_add(conn->fd, conn, FD_READ);

	} while (SOCKET_NONBLOCK); 
	// loop until accept no more(EAGAIN), or error happend
	
	return 0;
}
///////////////// SOCKET FUNCTION END ///////////// ]

///////////////// SINGLE FUNCTION START /////////// ]
void sig_chld(int signo)
{
	pid_t pid;
	int stat;
	while ((pid = waitpid(-1, &stat, WNOHANG)) > 0) {
		printf("sig_chld:child_terminated %d\n", pid);
	}
	return;
}
void sig_pipe(int signo)
{
	printf("sig_pipe:signo=%d\n", signo);
	return;
}
///////////////// SINGLE FUNCTION END ///////////// ]

int main(int argc, char *argv[])
{
	int ret = 0;

	// init signal handle
	signal(SIGCHLD, sig_chld);
	signal(SIGPIPE, sig_pipe);


	init_connect_list();

	ret = fdpoll_init();
	// printf("n=%d\n", n);

	int listen_fd = init_server();
	if (listen_fd < 0) {
		printf("main:init_server\n");
		return -6;
	}
	
	int num_ready = 0;
	int running = 1;
	connect_t * conn;

	while (running) {
		num_ready = fdpoll(FDPOLL_TIMEOUT);		
		printf("running:num_ready=%d\n", num_ready);

		// Is it a new connection?
		if (fdpoll_check(listen_fd)) {
			handle_newconnect(listen_fd);	
			continue;
		}

		// find the connections that need servicing
		while ((connect_t*)-1 != (conn = (connect_t*)fdpoll_get())) {
			if ((connect_t*)0 == conn) {
				printf("running:conn_error");
				continue;
			}
			ret = fdpoll_check(conn->fd);
			if (ret == 0) {
				// some unknow error happened
				do_disconnect(conn);
				continue;
			}

			if (conn->state == STATE_READ) {
				ret = do_read(conn);
				if (ret < 0) {
					do_disconnect(conn);
				}
				continue;
			}

			if (conn->state == STATE_WRITE) {
				ret = do_write(conn);
				if (ret < 0) {
					do_disconnect(conn);
				}
				continue;
			}

		}
	}

	return 0;
}



