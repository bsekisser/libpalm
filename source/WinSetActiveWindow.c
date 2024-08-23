#include "xWindow.h"

/* **** */

void WinSetActiveWindow(WinHandle winHandle)
{
	window_manager.enterWindowID = winHandle;
	window_manager.exitWindowID = window_manager.activeWindow;
}
