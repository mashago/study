
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

int main(int argc, char *argv[])
{

	int ret;
	char path[100];
	char program[100];

	/*
	sprintf(program, "/bin/ls");
	ret = execlp(program, "", "-l", "/private/tmp", (char *)0);
	printf("done %d\n", ret);
	*/

	sprintf(program, "./ss");
	ret = execlp(program, "", (char *)0);
	printf("done %d\n", ret);
	
	return 0;
}
