#include "Control.h"

/* **** */

#include "git/libbse/include/log.h"
//#include "git/libbse/include/unused.h"

/* **** */

void CtlSetUsable(ControlType* controlP, Boolean usable)
{
	if(!controlP) return;

	controlP->attr.usable = usable;
}
