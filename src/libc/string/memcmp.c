#include "../include/string.h"

int memcmp(const void* ptr1, const void* ptr2, size_t num)
{
	uint8_t* temp1 = (uint8_t *)(ptr1);
	uint8_t* temp2 = (uint8_t *)(ptr2);

	for(size_t i = 0; i < num; i++)
	{
		int comp = (*temp1) - (*temp2);

		if(comp != 0)
		{
			return comp;
		}

		temp1++;
		temp2++;
	}

	return 0;
}


