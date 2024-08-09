#pragma once

/* **** */

typedef struct libpalm_t** libpalm_h;
typedef struct libpalm_t* libpalm_p;

/* **** */

#include "xMemoryMgr.h"
#include "xResourceMgr.h"

/* **** */

#include "libbse/include/bin.h"

/* **** */

typedef struct libpalm_t {
	master_pointer_block_t master_pointer_block;
	resource_map_h current_resource;
}libpalm_t;

extern libpalm_p libpalm;
