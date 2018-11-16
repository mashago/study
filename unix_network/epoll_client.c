
#define _POSIX_SOURCE

#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/epoll.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <limits.h>
#include <netdb.h>

#define SERVER_IP	("127.0.0.1")
#define SERVER_PORT 7777

#define MAX_BUFFER 10

struct client_t
{
	int fd;
	char buffer[MAX_BUFFER];
	char* in_pos;
	char* out_pos;
};

struct client_t* new_client(int fd)
{
	struct client_t* cli = (struct client_t*)calloc(1, sizeof(struct client_t));
	cli->fd = fd;
	cli->in_pos = cli->buffer;
	cli->out_pos = cli->buffer;
	return cli;
}

int client_can_read_size(struct client_t *cli)
{
	return cli->buffer + MAX_BUFFER - cli->in_pos;
}

int client_can_write_size(struct client_t *cli)
{
	return cli->in_pos - cli->out_pos;
}

void client_write(struct client_t *cli)
{
	int count = write(cli->fd, cli->out_pos, client_can_write_size(cli));
	if (count > 0)
	{
		cli->out_pos += count;
		if (cli->out_pos == cli->in_pos)
		{
			cli->in_pos = cli->buffer;
			cli->out_pos = cli->buffer;
		}
	}
}

/////////////////////////////////////////

int prepare_addr(struct sockaddr_in *sin, const char *hostname, const int port)
{
	memset(sin, 0, sizeof(struct sockaddr_in));
	sin->sin_family = AF_INET;
	sin->sin_port = htons(port);
	
	char **pptr;
	struct hostent *hptr = gethostbyname(hostname);
	if (hptr == NULL)
	{
		printf("prepare_addr:no_such_host %s\n", hostname);
		return -1;
	}
	pptr = hptr->h_addr_list;

	// *pptr is first address in h_addr_list, is ready a numeric, just memcpy to sockaddr_in.sin_addr
	memcpy(&sin->sin_addr, *pptr, sizeof(struct in_addr));

	return 0;
}

int do_listen_stdin(int epfd)
{
	int fd = fileno(stdin);
	struct epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.fd = fd;
	epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
	return fd;
}

struct client_t* do_connect(int epfd)
{
	int fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
	struct sockaddr_in sin;
	int ret = prepare_addr(&sin, SERVER_IP, SERVER_PORT);
	if (ret != 0)
	{
		return NULL;
	}

	ret = connect(fd, (struct sockaddr *)&sin, sizeof(sin));
	if (ret != 0 && errno != EINPROGRESS)
	{
		printf("connect error %d\n", errno);
		return NULL;
	}

	struct client_t *cli = new_client(fd);
	struct epoll_event ev;
	ev.events = EPOLLIN | EPOLLOUT;
	ev.data.ptr = cli;
	epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);

	return cli;
}

void do_stdin(int epfd, int stdin_fd, struct client_t *cli)
{
	int size = client_can_read_size(cli);
	if (size == 0)
	{
		epoll_ctl(epfd, EPOLL_CTL_DEL, stdin_fd, NULL);
		return;
	}

	int count = read(stdin_fd, cli->in_pos, size);
	cli->in_pos += count;

	size = client_can_read_size(cli);
	if (size == 0)
	{
		epoll_ctl(epfd, EPOLL_CTL_DEL, stdin_fd, NULL);
	}
	
	struct epoll_event ev;
	ev.events = EPOLLIN | EPOLLOUT;
	ev.data.ptr = cli;
	epoll_ctl(epfd, EPOLL_CTL_MOD, cli->fd, &ev);
}

int do_read(int epfd, struct epoll_event *ev)
{
	struct client_t *cli = ev->data.ptr;
	int fd = cli->fd;

	char buffer[MAX_BUFFER+1];

	int count = read(fd, buffer, MAX_BUFFER);
	if (count == 0)
	{
		if (errno == EAGAIN)
		{
			return 0;
		}
		printf("do_read eof\n");
		return -1;
	}
	if (count < 0)
	{
		printf("do_read error %d\n", errno);
		return -1;
	}

	buffer[count] = '\0';	
	printf("%s", buffer);

	return 0;
}

void do_write(int epfd, struct epoll_event *ev)
{
	struct client_t *cli = ev->data.ptr;
	int fd = cli->fd;
	int size = client_can_write_size(cli);
	if (size == 0)
	{
		ev->events = EPOLLIN;
		epoll_ctl(epfd, EPOLL_CTL_MOD, fd, ev);
		return;
	}

	client_write(cli);

	size = client_can_read_size(cli);
	if (size > 0)
	{
		do_listen_stdin(epfd);
	}
	size = client_can_write_size(cli);
	if (size == 0)
	{
		ev->events = EPOLLIN;
		epoll_ctl(epfd, EPOLL_CTL_MOD, fd, ev);
	}
}

int main(int argc, char *argv[])
{
	printf("hello epoll client\n");

	int epfd = epoll_create(1024);
	if (epfd == -1)
	{
		printf("epoll_create error %d\n", errno);
		return 0;
	}

	int stdin_fd = do_listen_stdin(epfd);

	struct client_t *cli = do_connect(epfd);
	if (!cli)
	{
		printf("do_connect error\n");
		return 0;
	}

	const int maxevents = 2;
	struct epoll_event events[maxevents];
	while (1)
	{
		int nfds = epoll_wait(epfd, events, 10, -1);
		for (int i = 0; i < nfds; ++i)
		{
			if (events[i].data.fd == stdin_fd)
			{
				do_stdin(epfd, stdin_fd, cli);
			}
			else if (events[i].events & EPOLLIN)
			{
				int ret = do_read(epfd, &events[i]);
				if (ret != 0)
				{
					return 0;
				}
			}
			else if (events[i].events & EPOLLOUT)
			{
				do_write(epfd, &events[i]);
			}
		}
	}

	return 0;
}
