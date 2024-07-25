#include "MemoryMgr.h"

/* **** */

#include "chunk_t.h"
#include "utility.h"

/* **** */

#include "git/libbse/include/log.h"
//#include "git/libbse/include/unused.h"

/* **** */

MemPtr MemHandleLock(MemHandle h)
{
	void *const p = *(void**)h;

	chunk_p chunk = __MemHandle2Chunk(h);

	if(chunk->lockCount < 15)
		chunk->lockCount++;

	chunk->isLocked = true;

	return(p);
}
