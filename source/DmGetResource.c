#include "DataMgr.h"

/* **** */

#include "libpalm.h"
#include "resource_t.h"

/* **** */

#include "libbse/include/log.h"
#include "libbse/include/unused.h"

/* **** */

#include <inttypes.h>
#include <string.h>

/* **** */

MemHandle DmGetResource(DmResType type, DmResID resID)
{
	if(0) {
		UInt32 type_string[2] = { type, 0, };

		LOG_START("type: 0x%08lx -- %s", type, (Char*)&type_string);
		LOG_END(", resID: 0x%016" PRIxPTR, (uintptr_t)resID);
	}

	return((MemHandle)resource_get(libpalm->rsrc, type, resID));

	UNUSED(type, resID);
}
