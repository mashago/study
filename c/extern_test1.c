#include <stdio.h>

// in extern_test2.c
extern int et2_num;
extern int et2_print(const char*);

int main (int argc, char *argv[])
{
	
	printf("hello world\n");
	printf("et2_num=%d\n", et2_num);
	et2_print("hello et2");

	return 0;
}
