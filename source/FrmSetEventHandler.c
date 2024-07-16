#include "Form.h"

/* **** */

#include "git/libbse/include/log.h"
#include "git/libbse/include/unused.h"

/* **** */

void FrmSetEventHandler(FormType* formP, FormEventHandlerType* handler)
{
	if(!formP) return;

	formP->handler = handler;
}
