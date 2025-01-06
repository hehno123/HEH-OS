#ifndef _STRING_H
#define _STRING_H

#include <stddef.h>

size_t strlen(const char*);
void *memchr(const void*, int, size_t);
void *memcpy(void*, const void*, size_t);
void *memmove(void*, const void*, size_t);
char *strcpy(char*, const char*);
char *strncpy(char*, const char*, size_t);
char *strcat(char*, const char*);
char *strncat(char*, const char*, size_t);
int memcmp(const void*, const void*, size_t);

#endif
