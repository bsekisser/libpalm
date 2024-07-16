#include "Control.h"

/* **** */

#include "git/libbse/include/log.h"
//#include "git/libbse/include/unused.h"

/* **** */

void CtlDrawControl(ControlType* controlP)
{
	LOG("TODO");

	if(!controlP) return;

	controlP->attr.visible = true;
}
