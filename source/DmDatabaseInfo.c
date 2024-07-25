#include "DataMgr.h"

/* **** */

#include "dm_t.h"
#include "utility.h"

/* **** */

#include "git/libbse/include/log.h"
//#include "git/libbse/include/unused.h"

/* **** */

#include <string.h>

/* **** */

Err DmDatabaseInfo(const UInt16 cardNo, const LocalID dbID,
	Char *const nameP,
	UInt16 *const attributesP,
	UInt16 *const versionP,
	UInt32 *const crDateP,
	UInt32 *const modDateP,
	UInt32 *const bckUpDateP,
	UInt32 *const modNumP,
	LocalID *const appInfoIDP,
	LocalID *const sortInfoIDP,
	UInt32 *const typeP,
	UInt32 *const creatorP)
{
	dmdb_p db = __DmOpenDatabase(cardNo, dbID);

	PGET(appInfoIDP, db->appInfoID);
	PGET(attributesP, db->attributes);
	PGET(bckUpDateP, db->bckUpDate);
	PGET(crDateP, db->crDate);
	PGET(creatorP, db->creator);
	PGET(modDateP, db->modDate);
	PGET(modNumP, db->modNum);

	if(nameP)
		strncpy(nameP, db->name, sizeof(db->name) - 1);

	PGET(sortInfoIDP, db->sortInfoID);
	PGET(typeP, db->type);
	PGET(versionP, db->version);

	return(errNone); // TODO -- other errors may be returned
}
