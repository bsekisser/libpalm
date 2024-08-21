#include "xForm.h"
#include "xWindow.h"

/* **** */

#include "libbse/include/log.h"
#include "libbse/include/unused.h"

/* **** */

FormType* FrmGetFirstForm(void)
{
	WinPtr windowP = WinGetFirstWindow();

	while(windowP) {
		if(windowP->windowFlags.dialog)
			return((FormPtr)windowP);

		windowP = WinGetNextWindow(windowP);
	}

	return(0);
}
