#include "DataMgr.h"

/* **** */

#include "libbse/include/log.h"
#include "libbse/include/unused.h"

/* **** */

#include <inttypes.h>
#include <string.h>

/* **** */

MemHandle DmGetResource(DmResType type, DmResID resID)
{
	char type_string[5];
	*(UInt32*)type_string = type;
//	strncpy(type_string, (void*)&type, 4);
	type_string[4] = 0;

	LOG_START("type: 0x%08lx -- %s", type, type_string);
	LOG_END(", resID: 0x%016" PRIxPTR, (uintptr_t)resID);

	LOG("TODO"); return(0);

	UNUSED(type, resID);
}
