#include "DataMgr.h"
#include "MemoryMgr.h"

/* **** */

#include "dm_t.h"

/* **** */

#include "git/libbse/include/log.h"
//#include "git/libbse/include/unused.h"

/* **** */

MemHandle DmNewRecord(DmOpenRef const dbP, UInt16 *const atP, const UInt32 size)
{
	MemHandle h2dbr = MemHandleNew(sizeof(dmdbrecord_t) + size);

	LOG("TODO");

	dmdbrecord_p dbr = *(dmdbrecord_h)h2dbr;

	dbr->p2db = dbP;

	__DmSetRecordBusy(dbr, true);
	__DmSetRecordDirty(dbr, true);

	return(h2dbr);
}
