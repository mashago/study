#include <stdio.h>
#include "tool.h"

static int a = 1;
// extern int b = 2; // no a good idea to init and extern
int c = 3;

// has the same name function in main.c, so have to make this function static, avoid compile error
static int print_hello()
{
	printf("tool:print_hello()\n");
	return 0;
}

// extern int print_extern()
int print_extern()
{
	printf("tool:print_extern()\n");
	return 0;
}

// define in tool.h
int print_tool()
{
	print_hello();
	printf("a=%d c=%d\n", a, c);
	return 0;
}
