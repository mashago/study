
#include <sys/time.h>
#include <stdio.h>
#include <netinet/in.h> // for sockaddr_in
#include <netinet/tcp.h> // for TCP_NODELAY
#include <arpa/inet.h> // for inet_addr()
#include <unistd.h>
#include <string.h>


#define SERVER_HOST	"127.0.0.1"
#define SERVER_PORT	7720


int main(int argc, char *argv[])
{

	int ret;
	printf("socket_client:start\n");

	// init sockaddr_in
	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	int ip = inet_addr(SERVER_HOST);
	sin.sin_addr.s_addr = ip;
	sin.sin_port = htons(SERVER_PORT);

	// init socket
	int fd = socket(PF_INET, SOCK_STREAM, 0);
	printf("client:fd=%d\n", fd);
	if (fd < 0) {
		return -6;
	}
	int flag = 1;
	ret = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const void*)&flag, sizeof(flag));
	struct timeval timeout = {3, 0};
	ret = setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
	ret = setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
	flag = 1;
	ret = setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (const void*)&flag, sizeof(flag));


	// connect
	ret = connect(fd, (struct sockaddr*)&sin, sizeof(sin));
	if (ret != 0) {
		printf("client:connect_ret=%d\n", ret);
		return -16;
	}

	char buffer[101];

	if (NULL == fgets(buffer, 100, stdin)) {
		return 0;
	}

	/*
	sprintf(buffer, "hello");
	*/
	printf("client:buffer=%s\n", buffer);
	int size = write(fd, buffer, strlen(buffer));
	// int size = send(fd, buffer, strlen(buffer), 0);
	if (size == 0) {
		printf("client:write_size==0\n");
		return 0;
	}
	if (size < 0) {
		printf("client:write_size<0\n");
		return 0;
	}
	printf("client:write_size=%d\n", size);

	sleep(1);


	return 0;
}
