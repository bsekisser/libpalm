#include "xMemoryMgr.h"

/* **** */

#include "libbse/include/log.h"

/* **** */

#include <string.h>

/* **** */

master_pointer_block_t master_pointer_block;

/* **** */

__attribute__((constructor))
static void __memory_manager_init(void)
{
	LOG();

	memset(&master_pointer_block, 0, sizeof(master_pointer_block_t));
}

/* **** */
