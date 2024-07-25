#include "Form.h"

/* **** */

#include "libbse/include/log.h"
#include "libbse/include/unused.h"

/* **** */

extern FormType* gActiveForm;

/* **** */

UInt16 FrmGetActiveFormID(void)
{
	const UInt16 formID = gActiveForm ? gActiveForm->formId : 0;

	return(formID);
}
