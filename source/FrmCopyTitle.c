#include "Form.h"

/* **** */

#include "libbse/include/log.h"
#include "libbse/include/unused.h"

/* **** */

#include <string.h>

/* **** */

void FrmCopyTitle(FormType *const formP, const Char* newTitle)
{
//	char* dst = strncpy(formP->title, newTitle, form->reserved);
//	*dst = 0;

	if(formP->attr.usable) {
		if(formP->attr.visible)
			LOG("TODO");
;//			FrmDrawTitle(formP);
	}

	UNUSED(newTitle);
}
