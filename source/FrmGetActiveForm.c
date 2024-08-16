#include "xForm.h"
#include "xWindow.h"

/* **** */

#include "git/libbse/include/log.h"
#include "git/libbse/include/unused.h"

/* **** */

FormType* FrmGetActiveForm(void)
{
	WinPtr windowP = WinGetActiveWindow();

	return(windowP->windowFlags.dialog ? (FormPtr)windowP : 0);
}
