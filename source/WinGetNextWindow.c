#include "xWindow.h"

/* **** */

WinPtr WinGetNextWindow(WinPtr windowP)
{ return(windowP->nextWindow); }
