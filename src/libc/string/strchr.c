#include "../include/string.h"

const char* strchr(const char* str, int character)
{
	char* temp = (char*)(str);

	while (*temp != '\0') {
		if (*temp == (char)(character))
			return (const char*)(temp);
		
		temp++;
	}

	if(character == 0)
		return (const char*)(temp);

	return NULL;
}
