#include "DataMgr.h"

/* **** */

#include "xResourceMgr.h"

/* **** */

//#include "libbse/include/log.h"
//#include "libbse/include/unused.h"

/* **** */

MemHandle DmGetResource(DmResType type, DmResID resID)
{ return((MemHandle)resource_get(type, resID)); }
