#include "DataMgr.h"
#include "dm_t.h"

/* **** */

#include <string.h>

/* **** */

#include "libbse/include/log.h"
#include "libbse/include/unused.h"

/* **** */

MemHandle DmGetRecord(DmOpenRef dbP, UInt16 index)
{
	dmdbrecord_p dbr = __DmGetRecord(dbP, index);
	if(!dbr)
		return(0);

	Boolean busyWasSet = __DmSetRecordBusy(dbr, true);
	if(busyWasSet)
		return((MemHandle)__DmSetLastErr(dmErrRecordBusy));

	return((MemHandle)dbr);
}
