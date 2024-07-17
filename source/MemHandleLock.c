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
	MemPtr p = *(void**)h;

	chunk_p chunk = (chunk_p)((void*)p - (void*)MEMBER_OF(chunk_p, p));

	if(chunk->lockCount < 15)
		chunk->lockCount++;

	chunk->isLocked = true;

	return(p);
}
