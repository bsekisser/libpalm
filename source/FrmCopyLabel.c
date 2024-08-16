#include "xForm.h"

/* **** */

#include "libbse/include/log.h"
//#include "libbse/include/unused.h"

/* **** */

#include <string.h>

/* **** */

void FrmCopyLabel(FormType *const formP, const UInt16 labelID, const Char* newLabel)
{
	FormLabelType* labelP = (FormLabelType*)FrmGetObjectPtr(formP, labelID);

	char* dst = strncpy(labelP->text, newLabel, labelP->reserved);
	*dst = 0;

	if(formP->attr.usable) {
		if(labelP->attr.usable)
			LOG("TODO");
;//			FrmDrawLabel(formP, labelP);
	}
}
