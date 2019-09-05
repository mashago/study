
#include <stdio.h>

#ifdef _WIN32
#else
#include <dlfcn.h>
#endif

typedef void* (*dl_open_func)(void);

int main(int argc, char** argv)
{
    printf("hello cross\n");

    const char *libname = "lib/libmylib.so";
    void *dl = dlopen(libname, RTLD_NOW | RTLD_GLOBAL);
    if (!dl)
    {
        printf("dlopen fail\n");
        return 1;
    }
    printf("load lib success\n");

    dl_open_func open_func = (dl_open_func)dlsym(dl, "myopen");
    if (!open_func)
    {
        printf("dlsym fail %s\n", dlerror());
        return 1;
    }

    void *r1 = open_func();
    printf("r1=%p\n", r1);

    
    return 0;
}
