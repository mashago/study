#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>

/*
 * no name pipe:
 * int pipe(int fds[2]);
 * fds[0] for read, fds[1] for write
 *
 * FILE * popen(const char * command, const char *type);
 * command is a shell command, type is "r" or "w"
 * 
 * int pclose(FILE *fp);
 */

int list_file()
{
	FILE *fp;

	const int size = 1000;
	char buffer[size];

	fp = popen("ls", "r");
	while (fgets(buffer, size, fp) > 0) {
		printf("list_file:buffer=[%s]\n)", buffer);
	}
	pclose(fp);
	
	return 0;
}

int main(int argc, char **argv)
{
	int ret;
	printf("pipe_test\n");

	int pipe_fds[2];

	// pipe_fds[0] for read, pipe_fds[1] for write
	ret = pipe(pipe_fds);
	if (ret != 0) {
		printf("pipe:errno=%d\n", errno);
	}

	const int size = 100;
	char buffer[size];
	pid_t pid;
	if ((pid = fork()) == 0) {
		// child process
		close(pipe_fds[0]);
		strcpy(buffer, "hello pipe_test");
		ret = write(pipe_fds[1], buffer, strlen(buffer));
		if (ret < 0) {
			printf("child_write:errno=%d\n", errno);
			exit(0);
		}
		sleep(1);
		close(pipe_fds[1]);
		exit(0);
	}

	// father process
	close(pipe_fds[1]);
	ret = read(pipe_fds[0], buffer, sizeof(buffer));
	if (ret < 0) {
		printf("father_read:errno=%d\n", errno);
		exit(0);
	}
	buffer[ret] = '\0';
	printf("father:buffer=%s\n", buffer);
	close(pipe_fds[0]);

	waitpid(pid, NULL, 0);

	printf("\n");
	list_file();

	return 0;
}
