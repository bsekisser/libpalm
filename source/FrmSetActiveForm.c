#include "Form.h"

/* **** */

#include "libbse/include/log.h"
#include "libbse/include/unused.h"

/* **** */

extern FormType* gActiveForm;

/* **** */

void FrmSetActiveForm(FormType *const formP)
{ gActiveForm = formP; }
