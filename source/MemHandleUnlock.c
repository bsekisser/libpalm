#include "MemoryMgr.h"

/* **** */

#include "chunk_t.h"
#include "utility.h"

/* **** */

#include "git/libbse/include/log.h"
//#include "git/libbse/include/unused.h"

/* **** */

Err MemHandleUnlock(MemHandle h)
{
	if(!h)
		return(memErrInvalidParam);

	chunk_p chunk = __MemHandle2Chunk(h);

	if(!chunk->isHandle)
		return(memErrInvalidParam);

	if(chunk->lockCount)
		chunk->lockCount++;

	chunk->isLocked = (0 != chunk->lockCount);

	return(0);
}
