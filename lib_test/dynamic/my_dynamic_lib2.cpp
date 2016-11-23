#include <stdio.h>

// must extern "C" so that cpp can get the function
extern "C"
{
int print_dynamic_func(int a);
}

int print_dynamic_func(int a)
{
	printf("hello_dynamic_func: a=%d\n", a);
	return 0;
}
