#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>
#include <poll.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <limits.h>
#include <netdb.h>

#define SERVER_IP	("127.0.0.1")
#define SERVER_PORT 7777
// #define SERVER_IP	("t3.17kapai.com")
// #define SERVER_PORT 7710

/*
 * poll io:
 * 1.init poll list, struct pollfd pollfds[MAX_FDS], bzero and set all pollfds.fd is -1
 * 2.add lieten pollfds[i].fd = fd, pollfds[i].events = POLLIN/POLLOUT;
 * 3.num_ready = poll(&pollfds, nfds, timeout);
 * 4.if(pollfds[i].revents & POLLIN/POLLOUT/POLLERR...), pollfds[i] is ready to handle
 */

int do_read(int fd)
{
	int size;
	const int BUFFER_SIZE = 500;
	char buffer[BUFFER_SIZE+1];

	size = read(fd, buffer, BUFFER_SIZE);
	if (size == 0) {
		if (errno == EAGAIN) {
			printf("do_read:eagain\n");
			return 0;
		}
		printf("do_read:eof\n");
		return -1;
	}
	if (size < 0) {
		printf("do_read:error %d\n", errno);
		return -1;
	}
	if (size > 0) {
		buffer[size] = '\0';	
		printf("do_read:buffer=[%s]\n", buffer);
		return 0;
	}

	return 0;
}

int do_write(int fd)
{
	int size;
	const int BUFFER_SIZE = 500;
	char buffer[BUFFER_SIZE+1];

	fgets(buffer, BUFFER_SIZE, stdin);
	printf("do_write:buffer=[%s]\n", buffer);

	if (strcmp(buffer, "q\n") == 0) {
		shutdown(fd, SHUT_WR);
		return 0;
	}

	size = write(fd, buffer, sizeof(buffer));
	if (size < 0) {
		printf("do_write:error %d\n", errno);
		return -1;
	}
	return 0;
}

int prepare_addr(struct sockaddr_in *sin, const char *hostname, const int port)
{
	bzero(sin, sizeof(struct sockaddr_in));
	sin->sin_family = AF_INET;
	sin->sin_port = htons(port);
	
	char **pptr;
	struct hostent *hptr = gethostbyname(hostname);
	if (hptr == NULL) {
		printf("prepare_addr:no_such_host %s\n", hostname);
		return -1;
	}
	pptr = hptr->h_addr_list;

	// *pptr is first address in h_addr_list, is ready a numeric, just memcpy to sockaddr_in.sin_addr
	memcpy(&sin->sin_addr, *pptr, sizeof(struct in_addr));

	return 0;
}

int main(int argc, char *argv[])
{
	int ret;

	char host[50];
	int port = 0;

	if (argc >= 3) {
		snprintf(host, 50, "%s", argv[1]);
		port = atoi(argv[2]);
	} else {
		snprintf(host, 50, "%s", SERVER_IP);
		port = SERVER_PORT;
	}
	printf("host=%s port=%d\n", host, port);

	int nfds = 0;
	int max_fd = getdtablesize();
	printf("max_fd=%d\n", max_fd);

	/*
	 * struct pollfd {
	 * 		int		fd;
	 *		short	events;
	 *		short	revents;
	 * }
	 * events, revents: POLLIN, POLLOUT, POLLERR, POLLHUP, POLLNVAL
	 */
	struct pollfd poll_fds[max_fd]; // poll_fd array
	bzero(poll_fds, sizeof(poll_fds));
	for (int i=0; i<max_fd; i++) {
		poll_fds[i].fd = -1;
	}

	// fileno(stream) return stream's descriptor
	int stdinfd = fileno(stdin);
	// core logic
	poll_fds[nfds].fd = stdinfd; // add listen fd
	poll_fds[nfds].events = POLLIN; // stdin must use POLLIN
	nfds++;

	int fd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in sin;
	ret = prepare_addr(&sin, host, port);
	/*
	bzero(&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	inet_aton(SERVER_IP, &sin.sin_addr);
	sin.sin_port = htons(SERVER_PORT);
	*/

	ret = connect(fd, (struct sockaddr *)&sin, sizeof(sin));
	if (ret < 0) {
		printf("connect_refused errno=%d\n", errno);
		exit(0);
	}

	poll_fds[nfds].fd = fd;
	// poll_fds[nfds].events = POLLRDNORM;
	poll_fds[nfds].events = POLLIN;
	nfds++;
	
	int num_ready;
	int timeout = 10000;

	while (1) {

		// int poll(struct pollfd fds[], nfds_t nfds, int timeout);	
		// num_ready = poll(poll_fds, nfds, timeout);
		num_ready = poll(poll_fds, nfds, -1);
		printf("poll:num_ready=%d\n", num_ready);
		if (num_ready < 0) {
			printf("poll:errno=%d\n", errno);
		}

		if (poll_fds[0].revents & POLLIN) {
			printf("----stdin_fd\n");

			ret = do_write(fd);
			if (ret < 0) {
				printf("do_write:close_client\n");
				exit(0);				
			}
		}

		// if (poll_fds[1].revents & (POLLRDNORM | POLLERR)) {
		if (poll_fds[1].revents & (POLLIN | POLLERR)) {
			printf("----socket_fd\n");

			ret = do_read(fd);
			if (ret < 0) {
				printf("do_read:close_client\n");
				exit(0);				
			}

		}

	}

	return 0;
}
