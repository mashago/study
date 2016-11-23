#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <strings.h>
#include <unistd.h>
#include <poll.h>

#define UNIX_DOMAIN	"/tmp/usock.sock"

int main(int argc, char **argv)
{
	socklen_t socklen;
	struct sockaddr_un sun, cun;

	// init poll
	int nfds = 0;
	int max_fd = 2;
	struct pollfd pollfds[max_fd];
	bzero(pollfds, sizeof(pollfds));
	for (int i=0; i<max_fd; i++) {
		pollfds[i].fd = -1;
	}

	int listen_fd = socket(AF_LOCAL, SOCK_STREAM, 0);

	bzero(&sun, sizeof(sun));
	sun.sun_family = AF_LOCAL;
	strcpy(sun.sun_path, UNIX_DOMAIN);
	unlink(UNIX_DOMAIN);

	bind(listen_fd, (struct sockaddr*)&sun, sizeof(sun));
	listen(listen_fd, 5);

	pollfds[nfds].fd = listen_fd;
	pollfds[nfds].events = POLLIN;
	nfds++;

	int num_ready = 0;
	while (1) {
		num_ready = poll(pollfds, nfds, -1);
		printf("poll:num_ready=%d\n", num_ready);

		if (pollfds[0].revents & POLLIN) {
			printf("listen_fd:new connect\n");	
			socklen = sizeof(cun);
			int fd = accept(pollfds[0].fd, (struct sockaddr*)&cun, &socklen);
			if (fd < 0) {
				continue;
			}
			if (nfds == max_fd) {
				close(fd);
				continue;
			}
			pollfds[nfds].fd = fd;
			pollfds[nfds].events = POLLIN;
			nfds++;

			sleep(5);
		}

		if (pollfds[1].revents & POLLIN) {
			printf("client_fd\n");
			int size;
			const int buffer_size = 100;
			char buffer[buffer_size];

			size = read(pollfds[1].fd, buffer, buffer_size);
			if (size <= 0) {
				// close fd
				close(pollfds[1].fd);
				pollfds[1].fd = -1;
				nfds--;
				printf("nfds=%d\n", nfds);
				continue;
			}
			buffer[size] = '\0';
			printf("client_read:buffer=[%s]\n", buffer);

			size = write(pollfds[1].fd, buffer, strlen(buffer));
			if (size <= 0) {
				// close fd
				close(pollfds[1].fd);
				pollfds[1].fd = -1;
				nfds--;
				continue;
			}

		}

	}
	
	

	return 0;
}
