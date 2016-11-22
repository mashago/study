
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>
#include <strings.h>
#include <string.h>
#include <sys/stat.h>
#include <pthread.h>
#include <sys/wait.h>
#include <stdlib.h>

#include "Util.h"

#define SERVER_HOST "127.0.0.1"
#define SERVER_PORT 8080

int init_client(const char *host, int port)
{
	int fd = -1;

	fd = socket(PF_INET, SOCK_STREAM, 0);
	if (fd == -1)
	{
		error_exit("socket");
	}

	struct sockaddr_in sin;
	socklen_t sin_size = sizeof(sin);
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = inet_addr(host);
	sin.sin_port = htons(port);

	if (connect(fd, (struct sockaddr *)&sin, sin_size))
	{
		error_exit("connect");
	}

	return fd;
}

void send_request(int fd, const char *url)
{
	char buffer[1024];
	sprintf(buffer, "GET %s HTTP/1.1\r\n", url);
	send(fd, buffer, strlen(buffer), 0);
	sprintf(buffer, "Host: 192.168.2.67:8080\r\n");
	send(fd, buffer, strlen(buffer), 0);
	sprintf(buffer, "Cache-Control: max-age=0\r\n");
	send(fd, buffer, strlen(buffer), 0);
	sprintf(buffer, "Connection: keep-alive\r\n");
	send(fd, buffer, strlen(buffer), 0);
	sprintf(buffer, "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n");
	send(fd, buffer, strlen(buffer), 0);
	sprintf(buffer, "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_11_5) AppleWebKit/601.6.17 (KHTML, like Gecko) Version/9.1.1 Safari/601.6.17\r\n");
	send(fd, buffer, strlen(buffer), 0);
	sprintf(buffer, "Accept-Language: en-us\r\n");
	send(fd, buffer, strlen(buffer), 0);
	sprintf(buffer, "Accept-Encoding: gzip, deflate\r\n");
	send(fd, buffer, strlen(buffer), 0);
	sprintf(buffer, "DNT: 1\r\n");
	send(fd, buffer, strlen(buffer), 0);
	sprintf(buffer, "\n"); // end with '\n'
	send(fd, buffer, strlen(buffer), 0);
}

void handle_response(int fd)
{
	char buffer[1024];

	int size = 0;

	printf("---- recv http head start [\n");
	do
	{
		size = get_line(fd, buffer, sizeof(buffer));
		// printf("size=%d buffer=[%s]\n", size, buffer);
		printf("%s", buffer);
	}
	while (size > 0 && strcmp(buffer, "\n"));
	printf("---- recv http head end ]\n");


	printf("---- recv http body start [\n");
	do
	{
		size = get_line(fd, buffer, sizeof(buffer));
		printf("size=%d buffer=[%s]\n", size, buffer);
		// printf("%s", buffer);
	}
	while (size > 0 && strcmp(buffer, "\n"));
	printf("---- recv http body end ]\n");


}

void do_get(int fd, const char *url)
{
	send_request(fd, url);
}

void do_post(int fd, const char *url)
{
}

int main(int argc, char **argv)
{
	
	int client_fd;

	client_fd = init_client(SERVER_HOST, SERVER_PORT);

	const char *url = "/";

	do_get(client_fd, url);

	handle_response(client_fd);

	close(client_fd);
	return 0;
}
