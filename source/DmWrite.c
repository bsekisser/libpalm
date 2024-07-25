#include "DataMgr.h"
#include "dm_t.h"

/* **** */

#include <string.h>

/* **** */

#include "libbse/include/log.h"
#include "libbse/include/unused.h"

/* **** */

Err DmWrite(void *const recordP, const UInt32 offset, const void* srcP, const UInt32 bytes)
{
	dmdbrecord_p dbr = recordP;

	if(!dbr->h2db)
		return(-1); // TODO

	dmdb_p db = *dbr->h2db;
	if(!db)
		return(-1); // TODO

	// TODO -- validate record is part of database

	if(dbr->len < offset)
		return(-1); // TODO

	if(dbr->len < (offset + bytes))
		return(-1); // TODO

	memcpy(&dbr->data[offset], srcP, bytes);

	return(errNone);
}
