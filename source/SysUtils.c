#include "SysUtils.h"

/* **** */

#include "git/libbse/include/log.h"
#include "git/libbse/include/unused.h"

/* **** */

static unsigned long next = 1;

/* **** */

void SysCopyStringResource(Char* string, Int16 theID)
{
	LOG("TODO"); return;

	UNUSED(string, theID);
}

Int16 SysRandom(Int32 new_seed)
{
	next = ((new_seed ? (unsigned)new_seed : next) * 1103515245) + 12345;
	return((unsigned)(next / 65536) % sysRandomMax);
}

Char* SysStringByIndex(UInt16 resID, UInt16 index, Char* strP, UInt16 maxLen)
{
	LOG("TODO"); return(0);

	UNUSED(resID, index, strP, maxLen);
}
