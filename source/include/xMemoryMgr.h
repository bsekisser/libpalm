#pragma once

/* **** */

typedef struct master_pointer_t** master_pointer_h;
typedef struct master_pointer_t* master_pointer_p;

typedef struct master_pointer_block_t** master_pointer_block_h;
typedef struct master_pointer_block_t* master_pointer_block_p;

/* **** */

#include <stdlib.h>

/* **** */

typedef struct master_pointer_t {
	void* data;
//
	union {
		unsigned raw_flags;
		struct {
			unsigned locked:1;
			unsigned purgeable:1;
			unsigned resource:1;
		};
	};
//
	size_t alloc;
	size_t size;
}master_pointer_t;

#define __master_pointer_entries__ 64

typedef struct master_pointer_block_t {
	master_pointer_t entry[__master_pointer_entries__];
	master_pointer_block_p	next;
}master_pointer_block_t;

/* **** */

#include "libpalm.h"

/* **** */

#include "MemoryMgr.h"

/* **** */

master_pointer_p master_pointer_calloc(size_t size);
master_pointer_p master_pointer_find_handle(MemHandle const h, master_pointer_block_h const p2mb);
