#include "xControl.h"

/* **** */

//#include "git/libbse/include/log.h"
//#include "git/libbse/include/unused.h"

/* **** */

void CtlShowControl(ControlType* controlP)
{
	CtlSetUsable(controlP, true);
	CtlDrawControl(controlP);
}
