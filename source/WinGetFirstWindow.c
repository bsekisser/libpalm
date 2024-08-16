#include "xWindow.h"

/* **** */

//#include "libbse/include/log.h"
//#include "libbse/include/unused.h"

/* **** */

WinPtr WinGetFirstWindow(void)
{ return(window_manager.firstWindow); }
