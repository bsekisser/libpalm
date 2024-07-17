#pragma once

/* **** */

#include <sys/types.h>

/* **** */

typedef struct chunk_t* chunk_p;
typedef struct chunk_t {
	union {
		unsigned raw_flags;
		struct {
			unsigned isHandle:1;
			unsigned isLocked:1;
			unsigned lockCount:1;
		};
	};
//
	size_t alloc;
	size_t size;
//
	char p[];
}chunk_t;
