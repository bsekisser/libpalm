#include "xMemoryMgr.h"

/* **** */

#include "utility.h"

/* **** */

#include "git/libbse/include/log.h"
//#include "git/libbse/include/unused.h"

/* **** */

MemPtr MemHandleLock(MemHandle h)
{
	master_pointer_p mp = master_pointer_find_handle(h, 0);
	if(!mp)
		return(0);

	mp->locked = true;

	return(mp->data);
}
