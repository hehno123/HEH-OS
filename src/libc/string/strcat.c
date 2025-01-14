#include "../include/string.h"

char *strcat(char *destination, const char * source)
{
	char *dst = destination;
	char *src = (char *)(source);
	
	while (*dst != '\0')
		dst++;
		
	while (*src != '\0') {
	    *dst = *src;
	    dst++;
	    src++;
	}
	
	*dst = *src;
	return destination;
}
