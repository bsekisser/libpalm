#include "DataMgr.h"

/* **** */

#include "dm_t.h"

/* **** */

#include "git/libbse/include/log.h"
//#include "git/libbse/include/unused.h"

/* **** */

DmOpenRef DmOpenDatabase(UInt16 cardNo, LocalID dbID, UInt16 mode)
{
	dmdb_p db = __DmOpenDatabase(cardNo, dbID);

	__DmSetDatabaseAttr(db, dmHdrAttrOpen, true);

	db->mode = mode;
	return(db);
}
