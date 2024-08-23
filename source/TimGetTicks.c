#include "TimeMgr.h"

/* **** */

//#include "libbse/include/log.h"
//#include "libbse/include/unused.h"

/* **** */

#include <time.h>

/* **** */

UInt32 TimGetTicks(void)
{ return(clock()); }
