#include "xForm.h"

/* **** */

FormType* FrmGetFormPtr(const UInt16 formID)
{
	FormPtr formP = FrmGetFirstForm();

	while(formP) {
		if(formID == formP->formId)
			return(formP);

		formP = FrmGetNextForm(formP);
	}

	return(0);
}
