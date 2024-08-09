#include "DataMgr.h"

/* **** */

#include "libpalm.h"
#include "xResourceMgr.h"

/* **** */

#include "libbse/include/log.h"
#include "libbse/include/unused.h"

/* **** */

#include <inttypes.h>
#include <string.h>

/* **** */

MemHandle DmGet1Resource(DmResType type, DmResID resID)
{ return((MemHandle)resource_get1(libpalm->current_resource, type, resID)); }
