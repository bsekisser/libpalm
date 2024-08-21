#include "DataMgr.h"

/* **** */

#include "dm_t.h"

/* **** */

#include "git/libbse/include/log.h"
//#include "git/libbse/include/unused.h"

/* **** */

Err DmCreateDatabase(UInt16 cardNo, const Char* nameP, UInt32 creator, UInt32 type, Boolean resDB)
{
	LOG_START("cardNo: %u", cardNo);

	if(nameP)
		_LOG_(", name: %s", nameP);

	const char* cc = (char*)&creator;
	_LOG_(", creator: %c%c%c%c", cc[0], cc[1], cc[2], cc[3]);

	const char* tt = (char*)type;
	_LOG_(", type: %c%c%c%c", tt[0], tt[1], tt[2], tt[3]);

	LOG_END(", resDB: %u", resDB)

	LOG("TODO"); return(0);
}
