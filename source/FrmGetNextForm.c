#include "xForm.h"

/* **** */

FormPtr FrmGetNextForm(FormPtr formP)
{
	WinPtr windowP = ((WinPtr)formP) ?: WinGetFirstWindow();

	while(windowP) {
		if(windowP->windowFlags.dialog)
			return((FormPtr)windowP);

		windowP = windowP->nextWindow;
	}

	return(0);
}
