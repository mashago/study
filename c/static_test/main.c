#include <stdio.h>
#include "tool.h"

int print_hello()
{
	printf("main:print_hello()\n");
	return 0;
}

// extern tool.c function
extern int print_extern();

// extern int a;
// extern int b;
extern int c; // extern tool.c variable
// extern int d; // error, variable not exsits

int main(int argc, char **argv)
{
	print_tool();
	print_extern();
	print_hello();

	printf("c=%d\n", c);

	return 0;
}
