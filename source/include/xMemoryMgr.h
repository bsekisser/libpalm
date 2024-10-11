#pragma once

/* **** */

#include "sdk/include/Core/System/MemoryMgr.h"

/* **** */

#include "git/libbse/include/queue.h"

/* **** */

#include <stdlib.h>

/* **** */

typedef struct master_pointer_t* master_pointer_p;
typedef master_pointer_p const master_pointer_ref;

typedef struct master_pointer_block_t** master_pointer_block_h;
typedef struct master_pointer_block_t* master_pointer_block_p;
typedef master_pointer_block_p const master_pointer_block_ref;
typedef master_pointer_block_h const master_pointer_block_href;

typedef struct mem_chunk_t** mem_chunk_h;
typedef struct mem_chunk_t* mem_chunk_p;
typedef mem_chunk_p const mem_chunk_ref;
typedef mem_chunk_h const mem_chunk_href;

#define kMasterPointerCount 64
//#define kMasterPointerCount ((1 << 12) / sizeof(void*))

/* **** */

typedef struct master_pointer_t {
	void* data; // intentionally first, ie deref directly to payload
//
	mem_chunk_p chunk;
//
	struct {
		unsigned alloc:1;
	}flags;
}master_pointer_t;

typedef struct master_pointer_block_t {
	master_pointer_t master_pointer[kMasterPointerCount];
	qelem_t qnext;
}master_pointer_block_t;

typedef struct mem_chunk_t {
	size_t alloc;
	union {
		MemHandle handle;
		master_pointer_p master_pointer;
	};
	qelem_t qnext;
	size_t size;
//
	union {
		unsigned raw_flags;
		struct {
			unsigned free:1;
			unsigned heap:1;
//			unsigned is_master_pointer_block:1;
			unsigned lock:4;
			unsigned purgeable:1;
			unsigned resource:1;
		};
	};
//
	char data[];
}mem_chunk_t;

/* **** */

#include "MemoryMgr.h"

/* **** */

Boolean MasterPointerValidate(master_pointer_ref mp, mem_chunk_ref mc);
Boolean MemChunkValidate(mem_chunk_ref mc, MemPtr const p, master_pointer_ref mp);
MemPtr MemPtrNewClear(const size_t size);
int mem_handle_resource(MemHandle const h, const unsigned set_resource, const unsigned value);
