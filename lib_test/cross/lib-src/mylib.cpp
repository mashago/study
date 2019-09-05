
#include <stdio.h>

#ifdef _WIN32
#define MY_EXPORT __declspec (dllexport)
#else
#define MY_EXPORT
#endif


struct my_st
{
    int id;
};

extern "C" MY_EXPORT void * myopen()
{
    printf("hello myopen\n");
    my_st *node = new my_st();

    return (void *)node;
}
