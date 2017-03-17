
#include <sys/types.h>
#include <netinet/tcp.h> // for TCP_NODELAY
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h> // for sockaddr_in
#include <arpa/inet.h> // for inet_addr()
#include <string.h>
#include <unistd.h> // for sleep

#define SERVER_HOST	"127.0.0.1"
#define SERVER_PORT	7720


// XXX why undefine???
#ifndef SOL_TCP
#define SOL_TCP 6
#endif


int main(int argc, char * argv[])
{

	printf("socker_server:start\n");

	int ret;
	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	/*
	struct sockaddr_in {
		short int sin_family; 	// address family
		unsigned short int sin_port;
		struct in_addr sin_addr;	// internet address
		unsigned char size_zero[8];
	};
	struct in_addr {
		unsigned long s_addr; 
	};
	*/

	// init sockaddr_in
	sin.sin_family = AF_INET;
	int ip = inet_addr(SERVER_HOST);
	sin.sin_addr.s_addr = ip;
	sin.sin_port = htons(SERVER_PORT);

	// init socket
	// int fd = socket(sin.sin_family, SOCK_STREAM, 0);
	int fd = socket(sin.sin_family, SOCK_STREAM, IPPROTO_TCP);
	printf("server:fd=%d\n", fd);
	if (fd < 0) {
		return -6;
	}

	int flag = 1;
	ret = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const void*)&flag, sizeof(flag));

	// bind and listen 
	ret = bind(fd, (struct sockaddr*)&sin, sizeof(sin)); // change to sockaddr
	if (ret < 0) {
		printf("server:bind_error %d\n", ret);
		return ret;
	}

	int backlog = 1024;
	ret = listen(fd, backlog);
	if (ret != 0) {
		printf("server:listen_error %d\n", ret);
		return ret;
	}

	// accept client
	struct sockaddr_in c_sin;
	socklen_t len = sizeof(c_sin);
	int c_fd = accept(fd, (struct sockaddr*)&c_sin, &len);
	printf("server:client_fd=%d\n", c_fd);
	flag = 1;
	ret = setsockopt(c_fd, IPPROTO_TCP, TCP_NODELAY, (const void*)&flag, sizeof(flag));
#ifdef __linux__
	// keepalive
	flag = 1;
	ret = setsockopt(c_fd, SOL_SOCKET, SO_KEEPALIVE, &flag, sizeof(int));
	flag = 8; // max is 8 ?
	ret = setsockopt(c_fd, SOL_TCP, TCP_KEEPCNT, &flag, sizeof(int));
	flag = 3;  // after 3 seconds of idle, start keepalive
	ret = setsockopt(c_fd, SOL_TCP, TCP_KEEPIDLE, &flag, sizeof(int));
	flag = 3;  // interval is 3 seconds
	ret = setsockopt(c_fd, SOL_TCP, TCP_KEEPINTVL, &flag, sizeof(int));
#endif




	// read from client
	char buffer[101];
	// memset(buffer, 0, sizeof(buffer));
	buffer[0] = '\0';
	int size;
	size = read(c_fd, buffer, 100);
	if (size == 0) {
		printf("server:read_size==0\n");
		close(c_fd);
		return 0;
	}
	if (size < 0) {
		printf("server:read_size<0\n");
		close(c_fd);
		return -6;
	}
	// must add null terminlate
	buffer[size] = '\0'; 
	printf("server:size=%d buffer=%s\n", size, buffer);

	// in terminal client, after do enter send
	// , read buffer will = 'xxxx\r\n'
	// '\r' = 13, '\n' = 10
	printf("server:buffer:");
	for (int i=0; i<size; i++) {
		printf(" %d", buffer[i]);
	}
	printf("\n");

	sleep(2);
	return 0;
}
