#ifndef _UTIL_H_
#define _UTIL_H_

#include <stddef.h>

char *read_file(const char *path);
int  write_file(const char *path, const char *content, size_t length);
int  append_file(const char *path, const char *content, size_t length);

#endif // _UTIL_H_