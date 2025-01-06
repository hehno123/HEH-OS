#include "../include/string.h"

char *strncpy(char *destination, const char* source, size_t num)
{
	if(num > 0)
	{
		char* dst = destination;
		char* src = (char *)(source);
		char padding = 0;

		do
		{
			if(*src == '\0')
				padding = 1;
			
			*dst = (padding) ? '\0' : *src;
			dst++;
			src++;
			num--;

		} while (num != 0);
	}

	return destination;
}
