
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char **argv)
{
	printf("hello process_test\n");

	printf("parent pid=%d\n", getpid());
	printf("parent pgid=%d\n", getpgid(0));
	printf("parent sid=%d\n", getsid(0));

	pid_t pid;
	if ((pid = fork()) < 0) {
		printf("fork error\n");
		exit(1);
	} else if (pid == 0) {
		int ret;
		printf("\n");
		printf("child pid=%d\n", getpid());
		printf("child pgid=%d\n", getpgid(0));
		printf("child sid=%d\n", getsid(0));

		printf("\nafter setsid:\n");
		// if process is header in group, setsid() will fail to create a new session
		// if succsee, process will become a new group header, pgid, sid are pid
		ret = setsid();
		if (ret < 0) {
			printf("ret=%d errno=%d\n", ret, errno);
		}
		printf("child pid=%d\n", getpid());
		printf("child pgid=%d\n", getpgid(0));
		printf("child sid=%d\n", getsid(0));

		printf("\nafter setpgid:\n");
		// if process is already header in group, setpgid() will fail
		// if success, setpgid() will create a new process group, this process will become header, pgid is pid
		ret = setpgid(0, 0);
		if (ret < 0) {
			printf("ret=%d errno=%d\n", ret, errno);
		}
		printf("child pid=%d\n", getpid());
		printf("child pgid=%d\n", getpgid(0));
		printf("child sid=%d\n", getsid(0));

		exit(0);
	}
	sleep(3);

	return 0;
}
