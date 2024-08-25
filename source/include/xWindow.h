#pragma once

/* **** */

#include "config.h"

/* **** */

typedef struct window_manager_t* window_manager_p;

/* **** */

#define ALLOW_ACCESS_TO_INTERNALS_OF_WINDOWS

#include "Window.h"

/* **** */

typedef struct window_manager_t {
	WinHandle activeWindow;
	WinHandle drawWindow;
	WinHandle enterWindowID;
	WinHandle exitWindowID;
	WinHandle exitedWindowID;
	WinHandle firstWindow;
}window_manager_t;

/* **** */

extern window_manager_t window_manager;

/* **** */

void WindowMgrInit(void);
WinPtr WinGetNextWindow(WinPtr windowP);
WinHandle WinSetDrawWindow(WinHandle winHandle);
