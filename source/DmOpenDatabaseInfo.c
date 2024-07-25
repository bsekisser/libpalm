#include "DataMgr.h"

/* **** */

#include "dm_t.h"
#include "utility.h"

/* **** */

#include "git/libbse/include/log.h"
//#include "git/libbse/include/unused.h"

/* **** */

Err DmOpenDatabaseInfo(DmOpenRef dbP,
	LocalID *const dbIDP,
	UInt16 *const openCountP,
	UInt16 *const modeP,
	UInt16 *const cardNoP,
	Boolean *const resDBP)
{
	if(!dbP) return(-1);

	dmdb_p db = (void*)dbP;

	PGET(cardNoP, db->cardNo);
	PGET(dbIDP, db->dbID);
	PGET(modeP, db->mode);
	PGET(openCountP, db->openCount);
	PGET(resDBP, !!(db->attributes & dmHdrAttrResDB));

	return(errNone);
}
