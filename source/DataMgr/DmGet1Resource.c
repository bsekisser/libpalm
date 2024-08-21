#include "DataMgr.h"

/* **** */

#include "xResourceMgr.h"

/* **** */

//#include "libbse/include/log.h"
//#include "libbse/include/unused.h"

/* **** */

//#include <inttypes.h>
//#include <string.h>

/* **** */

MemHandle DmGet1Resource(DmResType type, DmResID resID)
{ return((MemHandle)resource_get1(current_resource, type, resID)); }
