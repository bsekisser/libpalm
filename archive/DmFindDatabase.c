#include "DataMgr.h"

/* **** */

#include "dm_t.h"

/* **** */

#include "git/libbse/include/log.h"
//#include "git/libbse/include/unused.h"

/* **** */

LocalID DmFindDatabase(UInt16 cardNo, const Char* nameP)
{
	if(nameP)
		LOG("cardNo: %u, name: %s", cardNo, nameP);

	LOG("TODO"); return(0);
}
