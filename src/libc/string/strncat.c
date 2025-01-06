#include "../include/string.h"

char *strncat(char *destination, const char* source, size_t num)
{
	char *dst = destination;
	char *src = (char *)(source);

	while(*dst != '\0')
	{
		dst++;
	}

	while(*dst != '\0' || num != 0)
	{
		*dst = *src;
		
		*dst++;
		*src++;
		num--;
	}

	*dst = '\0';
	return destination;
}
