#include "config.h"

#include "xField.h"
#include "MemoryMgr.h"

/* **** */

#include "libbse/include/log.h"
//#include "libbse/include/unused.h"

/* **** */

#include <assert.h>

/* **** */

void FldSetTextHandle(FieldType *const fldP, MemHandle textHandle)
{
	PEDANTIC(assert(fldP));

	LOG("TODO"); return;

	FldSetText(fldP, textHandle, 0, MemHandleSize(textHandle));
}
