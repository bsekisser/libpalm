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

Err DmSetDatabaseInfo(const UInt16 cardNo, const LocalID dbID,
	const Char* nameP,
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
	dmdb_p db = dmdbhdr_for_card_local(cardNo, dbID);

	PSET(appInfoIDP, db->appInfoID);
	PSET(attributesP, db->attributes);
	PSET(bckUpDateP, db->bckUpDate);
	PSET(crDateP, db->crDate);
	PSET(creatorP, db->creator);
	PSET(modDateP, db->modDate);
	PSET(modNumP, db->modNum);

	if(nameP)
		strncpy(db->name, nameP, sizeof(db->name) - 1);

	PSET(sortInfoIDP, db->sortInfoID);
	PSET(typeP, db->type);
	PSET(versionP, db->version);

	return(errNone); // TODO -- other errors may be returned
}
