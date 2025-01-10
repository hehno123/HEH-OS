#include "../include/string.h"

char* strrev(char* str)
{
	if(str == NULL)
		return NULL;
	
	int start = 0;
	int end = strlen(str) - 1;
	char temp;

	while(start < end)
	{
		temp = str[start];
		str[start] = str[end];
		str[end] = temp;

		start++;
		end--;
	}

	return str;
}

