#include "xMemoryMgr.h"

/* **** */

#include "chunk_t.h"
#include "utility.h"

/* **** */

#include "git/libbse/include/log.h"
//#include "git/libbse/include/unused.h"

/* **** */

Err MemHandleUnlock(MemHandle h)
{
	master_pointer_p mp = master_pointer_find_handle(h, 0);
	if(!mp)
		return(memErrInvalidParam);

	if(mp->lockCount)
		mp->lockCount--;

	mp->locked = (0 != mp->lockCount);

	return(errNone);
}
