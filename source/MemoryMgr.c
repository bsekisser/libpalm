#include "MemoryMgr.h"

/* **** */

#include "chunk_t.h"
#include "utility.h"

/* **** */

chunk_p __MemHandle2Chunk(MemHandle h)
{
	const void* p = *(void**)h;

	return((chunk_p)(p - (void*)OFFSET_OF(chunk_p, p)));
}
