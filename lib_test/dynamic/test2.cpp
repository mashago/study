#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

#define DYNAMIC_FILE "./libmy_dynamic2.so"
typedef int (*my_func_t)(int);

int main(int argc, char **argv)
{
	printf("hello dynamic test2\n");

	void *handle;
	char *error;
	my_func_t f;

	handle = dlopen(DYNAMIC_FILE, RTLD_LAZY);

	if (!handle)
	{
		fprintf(stderr, "1 %s\n", dlerror());
		exit(0);
	}

	dlerror();

	f = (my_func_t)dlsym(handle, "print_dynamic_func");
	// *(void **) &f = dlsym(handle, "print_dynamic_func");
	if ((error = dlerror()) != NULL)
	{
		fprintf(stderr, "2 %s\n", dlerror());
		exit(0);
	}

	f(13);

	dlclose(handle);

	return 0;
}
