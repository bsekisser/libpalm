#include "config.h"

#include "xForm.h"

/* **** */

#include "libbse/include/log.h"

/* **** */

FormPtr current_form;

/* **** */

__attribute__((constructor))
static void __form_globals_init(void)
{
	AT_INIT(LOG());

	current_form = 0;
}
