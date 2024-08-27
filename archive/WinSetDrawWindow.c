#include "xWindow.h"

/* **** */

WinPtr WinSetDrawWindow(WinHandle winHandle)
{
	WinHandle drawWindow = window_manager.drawWindow;

	winHandle->windowFlags.enabled = 1;

	window_manager.drawWindow = winHandle;

	return(drawWindow);
}
