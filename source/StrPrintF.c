#include "StringMgr.h"

/* **** */

#include <stdarg.h>
#include <stdio.h>

/* **** */

Int16 StrPrintF(Char* s, const Char* format, ...)
{
	va_list ap;

	va_start(ap, format);
	Int16 size = vsprintf(s, format, ap);
	va_end(ap);

	return(size);
}
