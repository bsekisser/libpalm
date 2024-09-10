#include "config.h"

#include "xWindow.h"

/* **** */

#include "libbse/include/log.h"

/* **** */

#include <string.h>

/* **** */

window_manager_t window_manager;

/* **** */

__attribute__((constructor))
static void __window_manager_globals_init(void)
{
	AT_INIT(LOG());

	memset(&window_manager, 0, sizeof(window_manager_t));
}
