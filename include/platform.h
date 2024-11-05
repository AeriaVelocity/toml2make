#ifndef PLATFORM_H

#define PLATFORM_H

#include <stdlib.h>

char *get_cwd(char *buf, size_t size);
char *get_full_path(const char *path);
int directory_is_empty(const char *path);

#endif