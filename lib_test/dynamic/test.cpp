#include <stdio.h>
#include "my_dynamic_lib.h"

int main(int argc, char **argv)
{
	printf("hello dynamic test\n");

	// function from dynamic lib
	print_dynamic_hello();

	return 0;
}
