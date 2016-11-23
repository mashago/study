#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <strings.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#define UNIX_DOMAIN	"/tmp/usock.sock"

int main(int argc, char **argv)
{
	int ret;
	struct sockaddr_un cun;

	int fd = socket(AF_LOCAL, SOCK_STREAM, 0);
	printf("fd=%d\n", fd);
	bzero(&cun, sizeof(cun));
	cun.sun_family = AF_LOCAL;
	strcpy(cun.sun_path, UNIX_DOMAIN);
	
	ret = connect(fd, (struct sockaddr *)&cun, sizeof(cun));
	if (ret < 0) {
		printf("connect:errno=%d\n", errno);
		exit(1);
	}

	int size;
	const int buffer_size = 100;
	char buffer[buffer_size];
	fgets(buffer, buffer_size, stdin);
	printf("fgets:buffer=[%s]\n", buffer);

	size = write(fd, buffer, strlen(buffer));
	if (size <= 0) {
		printf("write:errno=%d\n", errno);
		exit(1);
	}
	printf("write 1\n");

	size = write(fd, buffer, strlen(buffer));
	if (size <= 0) {
		printf("write:errno=%d\n", errno);
		exit(1);
	}
	printf("write 2\n");

	size = read(fd, buffer, sizeof(buffer));
	if (size <= 0) {
		printf("read:errno=%d\n", errno);
		exit(1);
	}

	buffer[size] = '\0';
	printf("read:buffer=[%s]\n", buffer);

	sleep(1);

	return 0;
	
}
