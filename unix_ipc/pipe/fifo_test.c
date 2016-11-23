#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define FIFO_PATH	"/tmp/fifo.1"
#define FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

/*
 * 1. create fifo
 * int mkfifo(const char *FIFO_PATH, mode_t FILE_MODE); // mkfifo includ O_CREATE | O_EXCL
 * 2. open it, half duplex, O_RDONLY or O_WRONLY
 * int open(const char *FIFO_PATH, int oflag);
 */

int main(int argc, char **argv)
{
	int ret;

	printf("fifo_test\n");

	// fifo = pipo with name
	ret = mkfifo(FIFO_PATH, FILE_MODE); // mkfifo includ O_CREATE | O_EXCL
	if (ret != 0)
	{
		if (errno == EEXIST)
		{
			printf("mkfifo:fifo exist\n");
		} 
		else
		{
			printf("mkfifo:errno=%d\n", errno);
			exit(0);
		}
	}

	printf("PIPO_BUF=%ld\n", pathconf(FIFO_PATH, _PC_PIPE_BUF));
	printf("OPEN_MAX=%ld\n", sysconf(_SC_OPEN_MAX));

	const int size = 100;
	char buffer[size];
	pid_t pid;
	if ((pid = fork()) == 0) {
		// child process
		// half duplex
		int write_fd = open(FIFO_PATH, O_WRONLY, 0);
		strcpy(buffer, "hello fifo_test");
		ret = write(write_fd, buffer, strlen(buffer));
		if (ret < 0) {
			printf("child_write:errno=%d\n", errno);
			exit(0);
		}
		sleep(1);
		exit(0);
	}

	// father process
	// half duplex
	int read_fd = open(FIFO_PATH, O_RDONLY, 0);
	ret = read(read_fd, buffer, sizeof(buffer));
	if (ret < 0) {
		printf("father_read:errno=%d\n", errno);
		exit(0);
	}
	buffer[ret] = '\0';
	printf("father:buffer=%s\n", buffer);

	// unlink(FIFO_PATH);
	exit(0);

	return 0;
}
