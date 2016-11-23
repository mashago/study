
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <strings.h>
#include <sys/un.h>
#include <errno.h>
#include <stdlib.h>

#define UNIX_DOMAIN	"/tmp/usock.sock"

int main(int argc, char **argv)
{
	int fd = socket(AF_LOCAL, SOCK_STREAM, 0);

	unlink(UNIX_DOMAIN);

	/*
	 * struct sockaddr_un {
	 * 		sa_family_t		sun_family;
	 * 		char			sun_path[104];
	 * }
	 *
	 */

	struct sockaddr_un sun;
	bzero(&sun, sizeof(sun));
	sun.sun_family = AF_LOCAL;
	strncpy(sun.sun_path, UNIX_DOMAIN, sizeof(sun.sun_path)-1);

	bind(fd, (struct sockaddr*)&sun, sizeof(sun));

	listen(fd, 5);

	
	pid_t pid = fork();
	if (pid == 0) {
		// child process
		close(fd);
		char buffer[100];
		struct sockaddr_un sun_c;	
		bzero(&sun_c, sizeof(sun_c));
		int cfd = socket(AF_LOCAL, SOCK_STREAM, 0);
		sun_c.sun_family = AF_LOCAL;
		strncpy(sun_c.sun_path, UNIX_DOMAIN, sizeof(sun_c.sun_path));
		connect(cfd, (struct sockaddr*)&sun, sizeof(sun_c));

		int size;
		int count;
		const char * str = "hello unix domain socket";
		count = strlen(str);

		size = write(cfd, &count, sizeof(count));
		printf("child_write1:size=%d\n", size);
		if (size < 0) {
			printf("child_write1:errno=%d\n", errno);
			exit(1);
		}

		
		strcpy(buffer, str);
		size = write(cfd, buffer, strlen(buffer));
		if (size < 0) {
			printf("child_write2:errno=%d\n", errno);
			exit(1);
		}

		count = 0;
		size = read(cfd, &count, sizeof(int));
		if (size < 0) {
			printf("child_read1:errno=%d\n", errno);
			exit(1);
		}
		printf("child_read1:count=%d\n", count);

		size = read(cfd, buffer, sizeof(buffer));
		if (size < 0) {
			printf("child_read2:errno=%d\n", errno);
			exit(1);
		}
		if (size == 0) {
			printf("child_read2:eof\n");
			exit(0);
		}
		buffer[size] = '\0';
		printf("child_read2:buffer=[%s]\n", buffer);
		sleep(1);


	} else {
		// father process
		char buffer[100];
		struct sockaddr_un sun_c;	
		socklen_t socklen = sizeof(sun_c);
		int cfd = accept(fd, (struct sockaddr*)&sun_c, &socklen);
		if (cfd < 0) {
			printf("accept:errno=%d\n", errno);
			exit(1);
		}
		int size;
		int count;
		size = read(cfd, &count, sizeof(int));
		printf("read1:size=%d\n", size);
		if (size < 0) {
			printf("read1:errno=%d\n", errno);
			exit(1);
		}
		printf("read1:count=%d\n", count);

		size = read(cfd, buffer, 100);
		if (size < 0) {
			printf("read2:errno=%d\n", errno);
			exit(1);
		}
		if (size == 0) {
			printf("read:eof\n");
			exit(0);
		}
		buffer[size] = '\0';
		printf("read2:buffer=[%s]\n", buffer);
		write(cfd, &count, sizeof(count));
		write(cfd, buffer, strlen(buffer));
		sleep(2);
	}

	
	return 0;
}
