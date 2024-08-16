#include "xForm.h"

/* **** */

#include "libbse/include/log.h"
#include "libbse/include/unused.h"

/* **** */

UInt16 FrmGetActiveFormID(void)
{
	FormPtr formP = FrmGetActiveForm();

	const UInt16 formID = formP ? formP->formId : 0;

	return(formID);
}
