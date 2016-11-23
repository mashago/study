#include <stdio.h>
#include "my_static_lib.h"

int main(int argc, char **argv)
{
	printf("hello static test\n");

	// function from static lib
	print_static_hello();

	return 0;
}
