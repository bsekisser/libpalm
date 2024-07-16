#include "StringMgr.h"

/* **** */

#include <string.h>

/* **** */

Char* StrNCopy(Char* dst, const Char* src, Int16 n)
{ return(strncpy(dst, src, n)); }
