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
#define SERVER_PORT 7711
// #define SERVER_IP	("t3.17kapai.com")
// #define SERVER_PORT 7710

/*
 * epoll io:
 * 1. int epollfd = epoll_create(int size);
 * 2. int epoll_ctl(int epollfd, int op, int fd, struct epoll_event *)
 * 		op: EPOLL_CTL_ADD, EPOLL_CTL_MOD, EPOLL_CTL_DEL
 * typedef union epoll_date {
 * 		void 			*ptr;
 * 		int 			fd;
 * 		__uint32_t		u32;
 * 		__uint64_t		u64;
 * 	} epoll_data_t;
 *
 * 	struct epoll_event {
 * 		__uint32_t		events; 
 * 		epoll_data_t	data;
 * 	}
 * 	epoll_event.events: EPOLLIN,EPOLLOUT,EPOLLERR,EPOLLHUP,EPOLLLT,EPOLLET
 *	3.num_ready = epoll_wait(epollfd, events_array, max_events, timeout);
 *
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

	// fgets(buffer, BUFFER_SIZE, stdin);
	size = read(fileno(stdin), buffer, BUFFER_SIZE);
	buffer[size] = '\0';
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

	int epfd = epoll_create(max_fd);

	/*
	 * typedef union epoll_date {
	 * 		void 			*ptr;
	 * 		int 			fd;
	 * 		__uint32_t		u32;
	 * 		__uint64_t		u64;
	 * 	} epoll_data_t;
	 *
	 * 	struct epoll_event {
	 * 		__uint32_t		events;
	 * 		epoll_data_t	data;
	 * 	}
	 *
	 */

	// fileno(stream) return stream's descriptor
	int stdinfd = fileno(stdin);
	// core logic
	struct epoll_event stdin_event;
	stdin_event.events = EPOLLIN;
	stdin_event.data.fd = stdinfd;
	epoll_ctl(epfd, EPOLL_CTL_ADD, stdinfd, &stdin_event);



	/*
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in sin;
	ret = prepare_addr(&sin, host, port);

	ret = connect(fd, (struct sockaddr *)&sin, sizeof(sin));
	if (ret < 0) {
		printf("connect_refused errno=%d\n", errno);
		exit(0);
	}

	struct epoll_event sock_event;
	stdin_event.events = EPOLLIN;
	stdin_event.data.fd = fd;
	epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &sock_event);
	*/

	
	int num_ready;
	int timeout = 10000;
	struct epoll_event events[10];

	while (1) {

		// int poll(struct pollfd fds[], nfds_t nfds, int timeout);	
		num_ready = epoll_wait(epfd, events, 10, -1);
		printf("epoll:errno=%d\n", errno);

		printf("epoll:num_ready=%d\n", num_ready);
		if (num_ready < 0) {
			printf("epoll:errno=%d\n", errno);
		}

		sleep(5);

		for (int i=0; i<num_ready; i++) {
			printf("events[%d].data.fd=%d\n", i, events[i].data.fd);
			if (events[i].events & EPOLLIN) {
				printf("EPOLLIN\n");
			} else if (events[i].events & EPOLLOUT) {
				printf("EPOLLOUT\n");
			} else if (events[i].events & EPOLLERR) {
				printf("EPOLLERR\n");
			}

			/*
			*/
			if (events[i].data.fd == stdinfd) {
				char buffer[1024];
				int size = read(events[i].data.fd, buffer, sizeof(buffer)-1);
				buffer[size] = '\0';
				printf("buffer=[%s]\n", buffer);
			}


			/*
			if (events[i].data.fd == stdinfd 
			&& (events[i].events & EPOLLIN)) {
				printf("----stdin_fd\n");
				ret = do_write(fd);
				if (ret < 0) {
					printf("do_write:close_client\n");
					exit(0);				
				}

				continue;
			}

			if (events[i].data.fd == fd
			&& (events[i].events & EPOLLIN)) {
				printf("----socket_fd\n");
				ret = do_read(fd);
				if (ret < 0) {
					printf("do_read:close_client\n");
					exit(0);				
				}
				continue;
			}
			*/
		}

	}

	return 0;
}
