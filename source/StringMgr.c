#include "StringMgr.h"

/* **** */

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* **** */

Int32 StrAToI(const Char* src)
{ return(atoi(src)); }

Int16 StrCaselessCompare(const Char* s1, const Char* s2)
{ return(strcasecmp(s1, s2)); }

Char* StrCat(Char* dst, const Char* src)
{ return(strcat(dst, src)); }

Int16 StrCompare(const Char* s1, const Char* s2)
{ return(strcmp(s1, s2)); }

Char* StrCopy(Char* dst, const Char* src)
{ return(strcpy(dst, src)); }

Char* StrIToA(Char* s, Int32 i)
{
	sprintf(s, "%ld", i);
	return(s);
}

UInt16 StrLen(const Char* src)
{ return(strlen(src)); }

Int16 StrNCompare(const Char* s1, const Char* s2, Int32 n)
{ return(strncmp(s1, s2, n)); }

Char* StrNCopy(Char* dst, const Char* src, Int16 n)
{ return(strncpy(dst, src, n)); }

Int16 StrPrintF(Char* s, const Char* format, ...)
{
	va_list ap;

	va_start(ap, format);
	Int16 size = vsprintf(s, format, ap);
	va_end(ap);

	return(size);
}

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
