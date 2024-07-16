#include "SysUtils.h"

/* **** */

static unsigned long next = 1;

Int16 SysRandom(Int32 new_seed)
{
	next = ((new_seed ? (unsigned)new_seed : next) * 1103515245) + 12345;
	return((unsigned)(next / 65536) % sysRandomMax);
}
