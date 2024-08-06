#pragma once

/* **** */

typedef struct libpalm_t** libpalm_h;
typedef struct libpalm_t* libpalm_p;

/* **** */

#include "xMemoryMgr.h"

/* **** */

#include "libbse/include/bin.h"

/* **** */

typedef struct libpalm_t {
	master_pointer_block_t master_pointer_block;
	bin_p rsrc;
}libpalm_t;

extern libpalm_p libpalm;
