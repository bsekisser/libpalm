#include "StringMgr.h"

/* **** */

#include <strings.h>

/* **** */

Int16 StrCaselessCompare(const Char* s1, const Char* s2)
{ return(strcasecmp(s1, s2)); }
