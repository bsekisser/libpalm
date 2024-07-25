#include "MemoryMgr.h"
#include "chunk_t.h"

/* **** */

#include "libbse/include/log.h"
#include "libbse/include/page.h"

/* **** */

#include <stdlib.h>

/* **** */

MemHandle MemHandleNew(UInt32 size)
{
	const size_t alloc = ((PAGE_SIZE - 1) + sizeof(chunk_t) + size) & ~PAGE_SIZE;
	chunk_p h = malloc(alloc); // TODO

	h->alloc = alloc;
	h->isHandle = true;
	h->size = size;

	return((MemHandle)&h->p);
}
