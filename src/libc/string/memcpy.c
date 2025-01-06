#include "../include/string.h"

void *memcpy(void* dest, const void* src, size_t num)
{
	if(num > 0)
	{
		uint8_t* destination = (uint8_t *)(dest);
		uint8_t* source = (uint8_t *)(src);
		
		do
		{
			*destination = *source;
			destination++;
			source++;
			num--;
		} while (num != 0);
	}

	return dest;
}
