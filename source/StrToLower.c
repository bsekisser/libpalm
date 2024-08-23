#include "StringMgr.h"

/* **** */

#include <ctype.h>

/* **** */

Char* StrToLower(Char* dst, const Char* src)
{
	unsigned int i = 0;

	while(src[i]) {
		dst[i] = tolower(src[i]);
		i++;
	}

	dst[i] = 0;

	return(dst);
}
