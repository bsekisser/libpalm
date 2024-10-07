#include "config.h"

#include "xField.h"
#include "MemoryMgr.h"

/* **** */

#include "libbse/include/log.h"
#include "libbse/include/unused.h"

/* **** */

#include <assert.h>
#include <inttypes.h>
#include <string.h>

/* **** */

struct config_field_t {
	struct {
		struct {
			unsigned entry:1;
		}at;
	}trace;
}field_config;

/* **** */

__attribute__((constructor))
static void __field_config_init(void)
{
	(void)memset(&field_config, 0, sizeof(field_config));

	field_config.trace.at.entry = 1;
}

/* **** */

void FldSetText(FieldType *const fldP, MemHandle textHandle, UInt16 offset, UInt16 size)
{
	if(!fldP) LOG("XXX fldP: 0x%016" PRIxPTR, (uintptr_t)fldP);

	if(textHandle)
		MemHandleLock(textHandle);

	LOG("TODO");

	return;
	UNUSED(offset, size);
}

void FldSetTextHandle(FieldType *const fldP, MemHandle textHandle)
{ return(FldSetText(fldP, textHandle, 0, MemHandleSize(textHandle))); }

