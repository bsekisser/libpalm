#include "xResourceMgr.h"

/* **** */

#include "libbse/include/log.h"

/* **** */

resource_map_h current_resource;

/* **** */

__attribute__((constructor))
static void __resource_manager_init(void)
{
	LOG();

	current_resource = 0;
}
