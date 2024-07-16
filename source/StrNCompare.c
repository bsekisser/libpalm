#include "StringMgr.h"

/* **** */

#include <string.h>

/* **** */

Int16 StrNCompare(const Char* s1, const Char* s2, Int32 n)
{ return(strncmp(s1, s2, n)); }
