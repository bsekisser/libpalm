#include "DataMgr.h"

/* **** */

#include "libbse/include/log.h"
#include "libbse/include/unused.h"

/* **** */

Err DmGetNextDatabaseByTypeCreator(const Boolean newSearch, DmSearchStatePtr const stateInfoP,
	const UInt32 type, const UInt32 creator, const Boolean onlyLatestVers,
	UInt16 *const cardNoP, LocalID *const dbIDP)
{
	LOG("TODO"); return (-1);

	UNUSED(newSearch, stateInfoP, type, creator, onlyLatestVers, cardNoP, dbIDP);
}
