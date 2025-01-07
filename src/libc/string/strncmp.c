#include "../include/string.h"

int strncmp(const char* str1, const char* str2, size_t num)
{
	if(num > 0)
	{
		char* temp1 = (char*)(str1);
		char* temp2 = (char*)(str2);

		while(*temp1 != '\0' && *temp2 != '\0')
		{
			if(num == 0)
				return 0;
			
			int comp = *temp1 - (*temp2);

			if(comp != 0)
			{
				return comp;
			}

			temp1++;
			temp2++;
			num--;
		}

		return ((*temp1) - (*temp2));
	}

	return 0;
}
