
#include <stdio.h>
#include <string.h>

#ifdef WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif
#include "dl_loader.h"

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

#endif
