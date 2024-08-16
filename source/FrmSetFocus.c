#include "xForm.h"

/* **** */

#include "libbse/include/log.h"
//#include "libbse/include/unused.h"

/* **** */

void FrmSetFocus(FormType* formP, UInt16 fieldIndex)
{
	formP->focus = fieldIndex;

	LOG("TODO");
}
