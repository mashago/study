
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <sys/epoll.h>

#define SERVER_PORT 7777
#define SERVER_IP "0.0.0.0"

#define MAX_BUFFER 10

// ping-pong client
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
void del_client(struct client_t* cli)
{
	close(cli->fd);
	free(cli);
}

int client_can_read_size(struct client_t *cli)
{
	return cli->buffer + MAX_BUFFER - cli->in_pos;
}

int client_read(struct client_t *cli)
{
	int count = read(cli->fd, cli->in_pos, client_can_read_size(cli));
	if (count > 0)
	{
		cli->in_pos += count;
	}
	return count;
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

void set_nonblock(int fd)
{
	int opts = fcntl(fd, F_GETFL);
	fcntl(fd, F_SETFL, (opts | O_NONBLOCK));
}

int do_listen(int epfd)
{
	// socket part
	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = inet_addr(SERVER_IP);
	sin.sin_port = htons(SERVER_PORT);

	int fd = socket(sin.sin_family, SOCK_STREAM, 0);
	if (fd == -1)
	{
		printf("socket error %d\n", errno);
		return -1;
	}

	int flag = 1;
	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(int));
	set_nonblock(fd);

	int ret = bind(fd, (struct sockaddr *)&sin, sizeof(sin));
	if (ret == -1)
	{
		printf("bind error %d\n", errno);
		return -1;
	}

	int backlog = 128;
	ret = listen(fd, backlog);
	if (ret == -1)
	{
		printf("listen error %d\n", errno);
		return -1;
	}

	// epoll part
	struct epoll_event ev;
	ev.data.fd = fd;
	ev.events = EPOLLIN;
	epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);

	return fd;
}

void do_accept(int epfd, int listen_fd)
{
	while (1)
	{
		struct sockaddr_in sin;
		memset(&sin, 0, sizeof(sin));
		socklen_t size = sizeof(sin);
		int client_fd = accept(listen_fd, (struct sockaddr*)&sin, &size);
		if (client_fd == -1)
		{
			if (errno == EAGAIN)
			{
				printf("accept EAGAIN\n");
			}
			else
			{
				printf("accept error %d\n", errno);
			}
			break;
		}

		printf("accept client_fd=%d\n", client_fd);
		set_nonblock(client_fd);
		struct epoll_event ev;
		ev.data.ptr = (void*)new_client(client_fd);
		ev.events = EPOLLIN;
		epoll_ctl(epfd, EPOLL_CTL_ADD, client_fd, &ev);
	}
}

void do_read(int epfd, struct epoll_event *ev)
{
	struct client_t *cli = ev->data.ptr;
	int fd = cli->fd;

	int size = client_can_read_size(cli);
	if (size == 0)
	{
		ev->events = EPOLLOUT;
		epoll_ctl(epfd, EPOLL_CTL_MOD, fd, ev);
		return;
	}

	int count = client_read(cli);
	if (count == 0)
	{
		epoll_ctl(epfd, EPOLL_CTL_DEL, fd, ev);
		del_client(cli);
		printf("read EOF, close socket\n");
		return;
	}

	if (count == -1)
	{
		if (errno == EAGAIN)
		{
			printf("read EAGAIN\n");
		}
		else
		{
			printf("read error %d\n", errno);
		}
		return;
	}

	ev->events = 0;
	size = client_can_read_size(cli);
	if (size > 0)
	{
		ev->events = ev->events | EPOLLIN;
	}
	size = client_can_write_size(cli);
	if (size > 0)
	{
		ev->events = ev->events | EPOLLOUT;
	}
	epoll_ctl(epfd, EPOLL_CTL_MOD, fd, ev);
}

void do_write(int epfd, struct epoll_event *ev)
{
	struct client_t *cli = ev->data.ptr;
	int fd = cli->fd;
	int size = client_can_write_size(cli);
	if (size == 0)
	{
		ev->events = ev->events & EPOLLIN;
		epoll_ctl(epfd, EPOLL_CTL_MOD, fd, ev);
		return;
	}

	client_write(cli);
	ev->events = 0;
	size = client_can_read_size(cli);
	if (size > 0)
	{
		ev->events = ev->events | EPOLLIN;
	}
	size = client_can_write_size(cli);
	if (size > 0)
	{
		ev->events = ev->events | EPOLLOUT;
	}
	epoll_ctl(epfd, EPOLL_CTL_MOD, fd, ev);
}

int main(int argc, char** argv)
{
	printf("hello epoll server\n");

	int epfd = epoll_create(1024);
	if (epfd == -1)
	{
		printf("epoll_create error %d\n", errno);
		return 0;
	}

	int listen_fd = do_listen(epfd);
	if (listen_fd == -1)
	{
		return 0;
	}

	const int maxevents = 20;
	struct epoll_event events[maxevents];
	while (1)
	{
		int nfds = epoll_wait(epfd, events, maxevents, -1);
		for (int i = 0; i < nfds; ++i)
		{
			if (events[i].data.fd == listen_fd)
			{
				do_accept(epfd, listen_fd);
			}
			else if (events[i].events & EPOLLIN)
			{
				do_read(epfd, &events[i]);
			}
			else if (events[i].events & EPOLLOUT)
			{
				do_write(epfd, &events[i]);
			}
		}
	}

	return 0;
}
