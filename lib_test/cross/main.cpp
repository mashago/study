
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#else
#include <dlfcn.h>
#endif

typedef void* (*dl_func)(void);

void *dl_load_lib(const char *path);
dl_func dl_load_func(void *lib, const char *sym);
void dl_unload_lib(void *lib);
char *dl_error(char *buffer, int size);

#define USE_DLOPEN 1

#ifndef WIN23

void *dl_load_lib(const char *path)
{
    void *lib = dlopen(path, RTLD_NOW | RTLD_GLOBAL);
	return lib;
}

dl_func dl_load_func(void *lib, const char *sym)
{
    dl_func open_func = (dl_func)dlsym(lib, sym);
	return open_func;
}

void dl_unload_lib(void *lib)
{
	dlclose(lib);
}

char *dl_error(char *buffer, int size)
{
	strncpy(buffer, dlerror(), size-1);
	buffer[size-1] = '\0';
	return buffer;
}

#else

void *dl_load_lib(const char *path)
{
}

dl_func dl_load_func(void *lib, const char *sym)
{
}

void dl_unload_lib(void *lib)
{
}

char *dl_error(char *buffer, int size)
{
}

#endif


#if __APPLE__
const char *libname = "lib/libmylib.dylib";
#elif WIN32
const char *libname = "lib/libmylib.dll";
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
        printf("dlopen fail %s\n", dl_error(error_buffer, buffer_size));
        return 1;
    }
    printf("load lib success\n");

    dl_func open_func = dl_load_func(lib, "myopen");
    if (!open_func)
    {
        printf("dlsym fail %s\n", dl_error(error_buffer, buffer_size));
        return 1;
    }

    void *r1 = open_func();
    printf("r1=%p\n", r1);

	dl_unload_lib(lib);
    
    return 0;
}
