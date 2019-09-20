
#include <stdio.h>
#include <string.h>

#ifdef WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

typedef void* (*dl_func)(void);

void *dl_load_lib(const char *path);
dl_func dl_load_func(void *lib, const char *sym);
void dl_unload_lib(void *lib);
char *dl_error(char *buffer, int size);

void pre_exit();

#ifndef WIN32

void *dl_load_lib(const char *path)
{
    void *lib = dlopen(path, RTLD_NOW | RTLD_GLOBAL);
	return lib;
}

dl_func dl_load_func(void *lib, const char *sym)
{
    dl_func func = (dl_func)dlsym(lib, sym);
	return func;
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

void pre_exit()
{
}

#else

void *dl_load_lib(const char *path)
{
	HMODULE lib = LoadLibraryExA(path, NULL, 0);
	return lib;
}

dl_func dl_load_func(void *lib, const char *sym)
{
	dl_func func = (dl_func)GetProcAddress((HMODULE)lib, sym);
	return func;
}

void dl_unload_lib(void *lib)
{
	FreeLibrary((HMODULE)lib);
}

#define snprintf(buffer, count, format, ...) do {_snprintf_s(buffer, count, count-1, format, ##__VA_ARGS__);} while (false)
char *dl_error(char *buffer, int size)
{
	int error = GetLastError();
	if (FormatMessageA(FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, error, 0, buffer, size, NULL))
	{
		return buffer;
	}

	snprintf(buffer, size, "system error %d\n", error);
	return buffer;
}

void pre_exit()
{
	getchar();
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

    dl_func open_func = dl_load_func(lib, "myopen2");
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
