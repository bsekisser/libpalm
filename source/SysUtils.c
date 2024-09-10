#include "config.h"

#include "SysUtils.h"

/* **** */

#include "git/libbse/include/log.h"
#include "git/libbse/include/unused.h"

/* **** */

#include <assert.h>

/* **** */

static unsigned long next = 1;

/* **** */

void SysCopyStringResource(Char* string, Int16 theID)
{
	PEDANTIC(assert(string));

	LOG("TODO"); return;

	if(!string) return;

	UNUSED(theID);
}

Int16 SysRandom(Int32 new_seed)
{
	next = ((new_seed ? (unsigned)new_seed : next) * 1103515245) + 12345;
	return((unsigned)(next / 65536) % sysRandomMax);
}

Char* SysStringByIndex(UInt16 resID, UInt16 index, Char* strP, UInt16 maxLen)
{
	PEDANTIC(assert(strP));

	LOG("TODO"); return(0);

	if(!strP) return(0);

	UNUSED(resID, index, maxLen);
}
