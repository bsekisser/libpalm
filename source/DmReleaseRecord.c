#include "DataMgr.h"

/* **** */

#include "dm_t.h"

/* **** */

#include "git/libbse/include/log.h"
//#include "git/libbse/include/unused.h"

/* **** */

Err DmReleaseRecord(DmOpenRef dbP, UInt16 index, Boolean dirty)
{
	if(index & ~dmMaxRecordIndex)
		return(dmErrIndexOutOfRange);

	dmdb_p db = dbP;

	dmdbrecord_p dbr = __DmGetRecord(db, index);

	__DmRecordDirty(dbr, &dirty);
	__DmRecordBusy(dbr, false);

	return(errNone);
}
