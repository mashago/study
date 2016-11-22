
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
#include "HttpServer.h"

#define SERVER_PORT 8080
#define SERVER_STRING "Server: mashahttpd/0.1.0\r\n"
#define DEFAULT_PAGE "index.html"

#define ISspace(x) isspace((int)(x))

int init_listen(int port)
{
	int fd = -1;

	fd = socket(PF_INET, SOCK_STREAM, 0);
	if (fd == -1)
	{
		error_exit("socket");
	}

	int flag = 1;
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(int)) < 0)
	{
		error_exit("reuse addr");
	}

	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_port = htons(port);

	if (bind(fd, (struct sockaddr *)&sin, sizeof(sin)) < 0)
	{
		error_exit("bind");
	}

	if (listen(fd, 1024) < 0)
	{
		error_exit("listen");
	}

	return fd;
}

int handle_client(int fd)
{
	// 1.get first line
	// 2.get http method, GET or POST
	// 3.get url
	// 4.handle request
	// 5.close client fd in the end
	
	const int MAX_BUFFER = 1024;
	char buffer[MAX_BUFFER];
	char method[255];
	char url[255];
	size_t i = 0;
	size_t j = 0;

	// 1.
	int size = get_line(fd, buffer, sizeof(buffer));
	if (size == 0)
	{
		close(fd);
		return 0;
	}

	// 2.
	i = 0; j = 0;
	while (!ISspace(buffer[j]) && (i < sizeof(method)-1) && (j < sizeof(buffer)))
	{
		method[i] = buffer[j];
		i++; j++;
	}
	method[i] = '\0';

	if (strcasecmp(method, "GET") && strcasecmp(method, "POST"))
	{
		send_unimplemented(fd);
		close(fd);
		return 0;
	}
	printf("method=%s\n", method);

	// 3.
	i = 0;
	while (ISspace(buffer[j]) && (j < sizeof(buffer)))
	{
		// skip space
		j++;
	}

	while (!ISspace(buffer[j]) && (i < sizeof(url)-1) && (j < sizeof(buffer)))
	{
		url[i] = buffer[j];
		i++; j++;
	}
	url[i] = '\0';
	printf("url=[%s]\n", url);

	// 4.
	if (strcasecmp(method, "GET") == 0)
	{
		do_get(fd, url);
	}

	if (strcasecmp(method, "POST") == 0)
	{
		do_post(fd, url);
	}

	printf("\n");

	close(fd);
	return 0;
}

int handle_client_test(int fd)
{
	
	char buffer[1024];

	int size = 0;

	printf("---- recv client start [\n");
	do
	{
		size = get_line(fd, buffer, sizeof(buffer));
		printf("%s", buffer);
	}
	while (size > 0 && strcmp(buffer, "\n"));
	printf("---- recv client end ]\n");

	if (size != 0)
	{
		send_simple_page(fd);
	}

	close(fd);
	return 0;
}

void do_get(int fd, const char *url)
{
	// 1.get file name 
	// 2.get param
	// 3.send target page to client
	
	char buffer[1024];
	const char *PATH_PREFIX = "file";
	char path[512];
	char param[512];
	char *ptr;
	size_t i = 0;

	path[0] = '\0';
	param[0] = '\0';
	
	// 1.
	ptr = path;
	ptr += sprintf(ptr, PATH_PREFIX);
	while (*url != '\0' && *url != '?')
	{
		*ptr = *url;
		ptr++; url++;
	}
	*ptr = '\0';
	if (path[strlen(path) - 1] == '/')
	{
		// set default page
		strcat(path, DEFAULT_PAGE);
	}
	printf("path=[%s]\n", path);
	
	// 2.
	i = 0;
	if (*url == '?')
	{
		// skip '?'
		url++;
	}
	while (*url != '\0')
	{
		param[i] = *url;
		i++; url++;
	}
	param[i] = '\0';
	printf("param=[%s]\n", param);


	int size = 0;
	do
	{
		size = get_line(fd, buffer, sizeof(buffer));
	}
	while (size > 0 && strcmp(buffer, "\n"));

	send_page(fd, path);
}

void do_post(int fd, const char *url)
{
	// TODO
	char buffer[1024];
	int size = 0;
	do
	{
		size = get_line(fd, buffer, sizeof(buffer));
	}
	while (size > 0 && strcmp(buffer, "\n"));
	send_unimplemented(fd);
}

void send_success_header(int fd)
{
	char buffer[1024];
	strcpy(buffer, "HTTP/1.0 200 OK\r\n");
	send(fd, buffer, strlen(buffer), 0);
	strcpy(buffer, SERVER_STRING);
	send(fd, buffer, strlen(buffer), 0);
	sprintf(buffer, "Content-Type: text/html\r\n");
	send(fd, buffer, strlen(buffer), 0);
	strcpy(buffer, "\r\n");
	send(fd, buffer, strlen(buffer), 0);
}

void send_not_found(int fd)
{
	char buffer[1024];

	sprintf(buffer, "HTTP/1.0 404 NOT FOUND\r\n");
	send(fd, buffer, strlen(buffer), 0);
	sprintf(buffer, SERVER_STRING);
	send(fd, buffer, strlen(buffer), 0);
	sprintf(buffer, "Content-Type: text/html\r\n");
	send(fd, buffer, strlen(buffer), 0);
	sprintf(buffer, "\r\n");
	send(fd, buffer, strlen(buffer), 0);
	sprintf(buffer, "<HTML><TITLE>Not Found</TITLE>\r\n");
	send(fd, buffer, strlen(buffer), 0);
	sprintf(buffer, "<BODY><P>404 NOT FOUND\r\n");
	send(fd, buffer, strlen(buffer), 0);
	sprintf(buffer, "</BODY></HTML>\r\n");
	send(fd, buffer, strlen(buffer), 0);
}

void send_unimplemented(int fd)
{
	char buffer[1024];

	sprintf(buffer, "HTTP/1.0 501 Method Not Implemented\r\n");
	send(fd, buffer, strlen(buffer), 0);
	sprintf(buffer, SERVER_STRING);
	send(fd, buffer, strlen(buffer), 0);
	sprintf(buffer, "Content-Type: text/html\r\n");
	send(fd, buffer, strlen(buffer), 0);
	sprintf(buffer, "\r\n");
	send(fd, buffer, strlen(buffer), 0);
	sprintf(buffer, "<HTML><HEAD><TITLE>Method Not Implemented\r\n");
	send(fd, buffer, strlen(buffer), 0);
	sprintf(buffer, "</TITLE></HEAD>\r\n");
	send(fd, buffer, strlen(buffer), 0);
	sprintf(buffer, "<BODY><P>HTTP request method not supported.\r\n");
	send(fd, buffer, strlen(buffer), 0);
	sprintf(buffer, "</BODY></HTML>\r\n");
	send(fd, buffer, strlen(buffer), 0);
}

void send_simple_page(int fd)
{
	char buffer[1024];
	strcpy(buffer, "HTTP/1.0 200 OK\r\n");
	send(fd, buffer, strlen(buffer), 0);
	strcpy(buffer, SERVER_STRING);
	send(fd, buffer, strlen(buffer), 0);
	sprintf(buffer, "Content-Type: text/html\r\n");
	send(fd, buffer, strlen(buffer), 0);
	strcpy(buffer, "\r\n");
	send(fd, buffer, strlen(buffer), 0);

	sprintf(buffer, "<HTML><TITLE>Simple Page</TITLE>\r\n");
	send(fd, buffer, strlen(buffer), 0);
	sprintf(buffer, "<BODY><P>This is a simple page.\r\n");
	send(fd, buffer, strlen(buffer), 0);
	sprintf(buffer, "<br>Hello World.\r\n");
	send(fd, buffer, strlen(buffer), 0);
	sprintf(buffer, "</BODY></HTML>\r\n");
	send(fd, buffer, strlen(buffer), 0);
}

void send_page(int fd, const char *filename)
{
	FILE *file = NULL;
	file = fopen(filename, "r");
	if (file == NULL)
	{
		send_not_found(fd);
	}
	else
	{
		send_success_header(fd);
		send_file(fd, file);
		fclose(file);
	}
}

void send_file(int fd, FILE *file)
{
	char buffer[1024];
	fgets(buffer, sizeof(buffer), file);
	while (!feof(file))
	{
		printf("size=%zu buffer=[%s]\n", strlen(buffer), buffer);
		send(fd, buffer, strlen(buffer), 0);
		fgets(buffer, sizeof(buffer), file);
	}
}

int main(int argc, char **argv)
{
	printf("hello http server\n");

	// 1.init server fd
	// 2.accept client fd
	// 3.handle client fd

	int server_fd = init_listen(SERVER_PORT);

	int client_fd = -1;
	struct sockaddr_in sin;
	socklen_t sin_size = sizeof(sin);

	while (true)
	{
		client_fd = accept(server_fd, (struct sockaddr *)&sin, &sin_size);
		if (client_fd == -1)
		{
			error_exit("accept");
		}
		handle_client(client_fd);
		// handle_client_test(client_fd);
	}

	close(server_fd);

	return 0;
}
