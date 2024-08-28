#include "MemoryMgr.h"

/* **** */

#include <string.h>

/* **** */

Err MemMove(void* dst, const void* src, Int32 n)
{ memmove(dst, src, n); return(0); }
