#include "xForm.h"

/* **** */

UInt16 FrmGetFormId(const FormType* formP)
{ return(formP ? formP->formId : 0); }
