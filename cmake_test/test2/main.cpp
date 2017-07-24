#include <stdio.h>
#include "dog.h"

int main(int argc, char **argv)
{
	printf("hello %s\n", argv[0]);
	Dog d = Dog("kittie");
	d.Talk();

	return 0;
}
