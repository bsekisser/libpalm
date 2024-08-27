#include "xForm.h"

/* **** */

#include "libbse/include/log.h"
#include "libbse/include/unused.h"

/* **** */

void FrmSetActiveForm(FormType *const formP)
{ WinSetActiveWindow(&formP->window); }
