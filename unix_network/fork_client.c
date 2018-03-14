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
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

#define TRY_FORK 1

#define SERVER_IP	("127.0.0.1")
#define SERVER_PORT 7711

int main(int argc, char *argv[])
{
	int ret;

	int fd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in sin;
	bzero(&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	inet_pton(AF_INET, SERVER_IP, &sin.sin_addr);
	sin.sin_port = htons(SERVER_PORT);


	ret = connect(fd, (struct sockaddr *)&sin, sizeof(sin));
	if (ret < 0) {
		printf("connect_refused errno=%d\n", errno);
		exit(0);
	}
	
	const int buff_size = 1000;
	char buff[buff_size];

	int num = 0;

#ifdef TRY_FORK
	pid_t pid;
	if ((pid = fork()) < 0)
	{
		printf("fork error\n");
		exit(1);
	}
#else
	int pid = 0;
#endif

	while (1)
	{
		++num;

		// send
		snprintf(buff, buff_size, "client send buff[%d][%d]", pid, num);
		int size = write(fd, buff, strlen(buff));
		if (size < 0)
		{
			printf("pid=%d errno=%d\n", pid, errno);
			sleep(2);
			continue;
		}
		printf("pid=%d size=%d\n", pid, size);

		// read
		size = read(fd, buff, buff_size);
		if (size < 0) {
			printf("pid=%d error %d\n", pid, errno);
			return -1;
		}
		if (size > 0) {
			buff[size] = '\0';	
			printf("pid=%d buff=[%s]\n", pid, buff);
		}

		sleep(2);
	}

	return 0;
}
