#pragma once

/* **** */

#include "config.h"

/* **** */

#define ALLOW_ACCESS_TO_INTERNALS_OF_WINDOWS

#include "sdk/include/Core/System/Window.h"

/* **** */

typedef struct window_manager_t* window_manager_p;

/* **** */

#ifndef ALLOW_ACCESS_TO_INTERNALS_OF_WINDOWS
	typedef struct WindowFlagsType {
		unsigned dialog:1;
		unsigned enabled:1;
		unsigned modal:1;
	}WindowFlagsType;

	typedef struct WindowType {
		WindowFlagsType windowFlags;
		RectangleType windowBounds;
		WinHandle nextWindow;
	}WindowType;
#endif

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

void _WinSetClip(WinPtr windowP, const RectangleType* rP);

/* **** */

void WindowMgrInit(void);
WinPtr WinGetNextWindow(WinPtr windowP);
WinHandle WinSetDrawWindow(WinHandle winHandle);
