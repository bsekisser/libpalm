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

	MemPtr p = *(void**)h;

	chunk_p chunk = (chunk_p)((void*)p - (void*)MEMBER_OF(chunk_p, p));

	if(!chunk->isHandle)
		return(memErrInvalidParam);

	if(chunk->lockCount)
		chunk->lockCount++;

	chunk->isLocked = (0 != chunk->lockCount);

	return(0);
}
