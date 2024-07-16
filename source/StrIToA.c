#include "StringMgr.h"

/* **** */

#include <stdio.h>

/* **** */

Char* StrIToA(Char* s, Int32 i)
{
	sprintf(s, "%ld", i);
	return(s);
}
