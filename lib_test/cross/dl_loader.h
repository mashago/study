#pragma once

typedef void* (*dl_func)(void);

void *dl_load_lib(const char *path);
dl_func dl_load_func(void *lib, const char *sym);
void dl_unload_lib(void *lib);
char *dl_error(char *buffer, int size);
