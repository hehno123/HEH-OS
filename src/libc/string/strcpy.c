#include "../include/string.h"

char *strcpy(char* dst, const char* src)
{
	size_t len = strlen(src) + 1;
	memcpy((void *)(dst), (const void *)(src), len);

	return dst;
}
