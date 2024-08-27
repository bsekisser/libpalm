#include "xForm.h"

/* **** */

#include "libbse/include/log.h"
#include "libbse/include/unused.h"

/* **** */

UInt16 FrmGetActiveFormID(void)
{ return(FrmGetFormId(FrmGetActiveForm())); }
