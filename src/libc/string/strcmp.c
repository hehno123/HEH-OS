#include "../include/string.h"

int strcmp(const char* str1, const char* str2)
{
	char* temp1 = (char*)(str1);
	char* temp2 = (char*)(str2);

	while(*temp1 != '\0' && *temp2 != '\0')
	{
		int comp = *temp1 - (*temp2);

		if(comp != 0)
		{
			return comp;
		}

		temp1++;
		temp2++;
	}

	return ((*temp1) - (*temp2));
}
