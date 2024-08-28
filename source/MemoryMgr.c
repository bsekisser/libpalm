#include "xMemoryMgr.h"

/* **** */

#include "chunk_t.h"
#include "utility.h"

/* **** */

#include "libbse/include/log.h"

/* **** */

#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

/* **** */

master_pointer_p master_pointer_calloc(size_t size)
{
	master_pointer_block_p mpb = &master_pointer_block;
	unsigned count = 0;

	while(mpb) {
if(0)	LOG("mpb: 0x%016" PRIxPTR, (uintptr_t)mpb);

		for(unsigned x = 0; x < __master_pointer_block_entries__; x++) {
			const master_pointer_p mp = &mpb->entry[x];

if(0)		LOG("mp: 0x%016" PRIxPTR, (uintptr_t)mp);

			if(!mp->alloc) {
				void* data = calloc(1, size);

if(0) {
				LOG_START("data: 0x%016" PRIxPTR, (uintptr_t)data);
				LOG_END(", size: 0x%08zx", (uintptr_t)size);
}

				if(data) {
					mp->alloc = size;
					mp->data = data;
					mp->size = size;
					return(mp);
				}
			}

			count++;
		}

		const master_pointer_block_p last_mpb = mpb;
		mpb = mpb->next;

		if(!mpb) {
			mpb = calloc(1, sizeof(master_pointer_block_t));

if(1)		LOG("calloc_mpb(%u): 0x%016" PRIxPTR, count, (uintptr_t)mpb);

			if(mpb)
				last_mpb->next = mpb;
		}
	}

	return(0);
}

master_pointer_p master_pointer_find_handle(MemHandle const h, master_pointer_block_h h2mpb)
{
	master_pointer_block_p mpb = &master_pointer_block;

	for(; mpb; mpb = mpb->next) {
if(0)	LOG("mpb: 0x%016" PRIxPTR, (uintptr_t)mpb);

		for(unsigned x = 0; x < __master_pointer_block_entries__; x++) {
			const master_pointer_p mp = &mpb->entry[x];

if(0)		LOG("mp: 0x%016" PRIxPTR, (uintptr_t)mp);

			if((void*)h == mp) {
				if(h2mpb)
					*h2mpb = mpb;

				return(mp);
			}
		}
	}

	return(0);
}

/* **** */

Err MemHandleFree(MemHandle h)
{
	master_pointer_p mp = master_pointer_find_handle(h, 0);
	if(!mp)
		return(memErrInvalidParam);

	if(mp->alloc)
		free(mp->data);

	memset(mp, 0, sizeof(master_pointer_t));

	return(errNone);
}


MemPtr MemHandleLock(MemHandle h)
{
	master_pointer_p mp = master_pointer_find_handle(h, 0);
	if(!mp)
		return(0);

	mp->locked = true;
	mp->lockCount++;

	return(mp->data);
}

MemHandle MemHandleNew(UInt32 size)
{
	master_pointer_p mp = master_pointer_calloc(size);

	mp->size = size;

	return((MemHandle)mp);
}

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

Err MemMove(void* dst, const void* src, Int32 n)
{ memmove(dst, src, n); return(0); }

Err MemPtrUnlock(MemPtr p)
{
	chunk_p chunk = (chunk_p)((void*)p - (void*)OFFSET_OF(chunk_p, p));

	if(chunk->lockCount)
		chunk->lockCount--;

	chunk->isLocked = (0 != chunk->lockCount);

	return(errNone);
}
