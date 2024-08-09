#include "xMemoryMgr.h"

/* **** */

#include "utility.h"

/* **** */

#include "libbse/include/log.h"

/* **** */

#include <inttypes.h>
#include <stdlib.h>

/* **** */

master_pointer_p master_pointer_calloc(size_t size)
{
	master_pointer_block_p mpb = &libpalm->master_pointer_block;

	while(mpb) {
if(0)	LOG("mpb: 0x%016" PRIxPTR, (uintptr_t)mpb);

		for(unsigned x = 0; x < __master_pointer_entries__; x++) {
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
		}

		const master_pointer_block_p last_mpb = mpb;
		mpb = mpb->next;

		if(!mpb) {
			mpb = calloc(1, sizeof(master_pointer_block_t));

if(1)		LOG("calloc mpb: 0x%016" PRIxPTR, (uintptr_t)mpb);

			if(mpb)
				last_mpb->next = mpb;
		}
	}

	return(0);
}

master_pointer_p master_pointer_find_handle(MemHandle const h, master_pointer_block_h h2mpb)
{
	master_pointer_block_p mpb = &libpalm->master_pointer_block;

	for(; mpb; mpb = mpb->next) {
if(0)	LOG("mpb: 0x%016" PRIxPTR, (uintptr_t)mpb);

		for(unsigned x = 0; x < __master_pointer_entries__; x++) {
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
