#include "MemoryMgr.h"

/* **** */

#include "chunk_t.h"
#include "utility.h"

/* **** */

#include "git/libbse/include/log.h"
//#include "git/libbse/include/unused.h"

/* **** */

Err MemPtrUnlock(MemPtr p)
{
	LOG("TODO"); return(0);

	chunk_p chunk = (chunk_p)((void*)p - (void*)MEMBER_OF(chunk_p, p));

	if(chunk->lockCount)
		chunk->lockCount--;

	chunk->isLocked = (0 != chunk->lockCount);
}
