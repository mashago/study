#include <stdio.h>

int main(int argc, char **argv)
{
	printf("hello test\n");

#ifdef DEBUG_FLAG
	printf("DEBUG_FLAG on\n");
#else
	printf("DEBUG_FLAG off\n");
#endif

	return 0;
}
