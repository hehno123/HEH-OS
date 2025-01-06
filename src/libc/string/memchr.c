#include "../include/string.h"

void *memchr(const void *ptr, int value, size_t num)
{
	if(num > 0)
	{
		const uint8_t *p = ptr;

		do {
			if(*p == (uint8_t)(value))
			{
				return ((void *)(p)); 
			}

			p++;
			n--;
		} while(n != 0);
	}

	return NULL;
}
