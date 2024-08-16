#include "xForm.h"

/* **** */

#include "libbse/include/log.h"
#include "libbse/include/unused.h"

/* **** */

#include <string.h>

/* **** */

void FrmSetTitle(FormType *const formP, Char *const newTitle)
{
//	formP->title = newTitle;

	if(formP->attr.usable) {
		if(formP->attr.visible)
			LOG("TODO");
;//			FrmDrawLabel(formP, labelP);
	}

	UNUSED(newTitle);
}
