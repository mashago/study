
#include <stdio.h>
#include <string.h>

#ifdef WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif
#include "dl_loader.h"

void pre_exit();

#ifdef WIN32
void pre_exit()
{
	getchar();
}
#else
void pre_exit()
{
}
#endif


#if __APPLE__
const char *libname = "lib/libmylib.dylib";
#elif WIN32
const char *libname = "../lib/mylib.dll";
#else
const char *libname = "lib/libmylib.so";
#endif

int main(int argc, char** argv)
{
    printf("hello cross\n");

	const int buffer_size = 100;
	char error_buffer[buffer_size];

    void *lib = dl_load_lib(libname);
    if (!lib)
    {
        printf("dl_load_lib fail %s\n", dl_error(error_buffer, buffer_size));
		pre_exit();
        return 1;
    }
    printf("load lib success\n");

    dl_func open_func = dl_load_func(lib, "myopen");
    if (!open_func)
    {
        printf("dl_load_func fail %s\n", dl_error(error_buffer, buffer_size));
		pre_exit();
        return 1;
    }

    void *r1 = open_func();
    printf("r1=%p\n", r1);

	dl_unload_lib(lib);

	pre_exit();

    return 0;
}
