#include "DataMgr.h"

/* **** */

#include "dm_t.h"

/* **** */

#include "git/libbse/include/log.h"
//#include "git/libbse/include/unused.h"

/* **** */

Err DmCloseDatabase(DmOpenRef dbP)
{
	LOG("TODO"); return(0);

	dmdb_p db = dbP;

	if(db->attributes & dmHdrAttrRecyclable)
		DmDeleteDatabase(db->cardNo, db->dbID);
}
