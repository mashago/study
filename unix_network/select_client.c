#ifdef __linux__
#define _GNU_SOURCE
#endif
#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>
#include <strings.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>

#define SERVER_IP	("127.0.0.1")
#define SERVER_PORT 7777
// #define SERVER_IP	("114.215.133.1")
// #define SERVER_PORT 7710
// #define SERVER_IP	("192.168.2.5")
// #define SERVER_PORT 43731


/*
 * select io: 
 * 1.init fd_sets, fd_set readset, fd_set writeset, FD_ZERO(&readset), FDZERO(&writeset)
 * 2.FD_SET(fd, &fdset) // add listen
 * 3.FD_CLR(fd, &fdset) // delete listen
 * 4.num_ready = select(int nfds, &readset, &writeset, &exceptset, &timeval); // core logic
 * 5.FD_ISSET(fd, &fdset) // check fd if is ready
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

int main(int argc, char *argv[])
{
	int ret;

	// fileno(stream) return stream's descriptor
	int stdinfd = fileno(stdin);

	int fd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in sin;
	bzero(&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	inet_pton(AF_INET, SERVER_IP, &sin.sin_addr);
	sin.sin_port = htons(SERVER_PORT);

	// set nonblock
	int opts;
	opts = fcntl(fd, F_GETFL);
	opts = opts | O_NONBLOCK;
	fcntl(fd, F_SETFL, opts);

	ret = connect(fd, (struct sockaddr *)&sin, sizeof(sin));
	if (ret < 0) {
		printf("connect_refused errno=%d\n", errno);
		exit(0);
	}
	
	int num_ready;

	int nfds;
	fd_set rset; // save read fds
	FD_ZERO(&rset); // must empty fd_set

	struct timeval timeout;

	while (1) {
		FD_SET(stdinfd, &rset); // listen socket fd
		FD_SET(fd, &rset);	 // listen stdin fd

		bzero(&timeout, sizeof(timeout)); // must bzero it
		timeout.tv_sec = 10;
		// The first nfds descriptors are checked in each set; i.e., the descriptors from 0 through nfds-1 in the descriptor sets are examined.  (Example: If you have set two file descriptors "4" and "17", nfds should  not be "2", but rather "17 + 1" or "18".)
		nfds = (stdinfd > fd) ? (stdinfd+1) : (fd+1);

		// int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *errorfds, struct timeval * timosut);
		num_ready = select(nfds, &rset, NULL, NULL, &timeout);
		printf("select:num_ready=%d\n", num_ready);
		if (num_ready < 0) {
			printf("select:errno=%d\n", errno);
		}

		if (FD_ISSET(stdinfd, &rset)) {
			printf("----stdin_fd\n");

			ret = do_write(fd);
			if (ret < 0) {
				printf("do_write:close_client\n");
				exit(0);				
			}
		}

		if (FD_ISSET(fd, &rset)) {
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
