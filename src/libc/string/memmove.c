#include "../include/string.h"

void *memmove(void *destination, const void *source, size_t num)
{
	if(num > 0)
	{
		uint8_t temp[num];
		memcpy((void*)(temp[num]), source, num);
		memcpy(destination, (const void*)(temp), num);
	}

	return destination;
}
