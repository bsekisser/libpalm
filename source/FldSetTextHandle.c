#include "Field.h"
#include "MemoryMgr.h"

/* **** */

#include "libbse/include/log.h"
//#include "libbse/include/unused.h"

/* **** */

void FldSetTextHandle(FieldType *const fldP, MemHandle textHandle)
{
	LOG("TODO"); return;

	FldSetText(fldP, textHandle, 0, MemHandleSize(textHandle));
}
