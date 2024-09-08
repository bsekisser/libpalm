#pragma once

/* **** */

#include "config.h"

/* **** */

#define ALLOW_ACCESS_TO_INTERNALS_OF_WINDOWS

#include "sdk/include/Core/System/Window.h"

#include "xEvent.h"

/* **** */

typedef struct window_manager_t* window_manager_p;

/* **** */

typedef struct window_manager_t {
	WinHandle enterWindowID;
	WinHandle exitWindowID;
	WinHandle exitedWindowID;
}window_manager_t;

/* **** */

extern window_manager_t window_manager;

/* **** */

void _WinCreateWindow(WinPtr windowP, const RectangleType* bounds, FrameType frame,
	Boolean modal, Boolean focusable);

void _WinSetClip(WinPtr windowP, const RectangleType* rP);

/* **** */

void _WinSetVisible(WinHandle winHandle, Boolean visible);

/* **** */

void WindowMgrInit(void);
void WinDrawWindow(WinPtr const windowP);
WinPtr WinGetNextWindow(WinPtr windowP);
Boolean WinHandleEvent(EventPtr eventP);
WinHandle WinSetDrawWindow(WinHandle winHandle);
