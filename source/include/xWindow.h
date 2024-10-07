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
	WinHandle activeWindow;
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

void WindowMgrInit(void);
void WinDrawWindow(WinPtr const windowP);
void WinEnterWindowEvent(const struct _WinEnterEventType *const winEnter);
void WinExitWindowEvent(const struct _WinExitEventType *const winExit);
WinPtr WinGetNextWindow(WinPtr windowP);
Boolean WinHandleEvent(EventPtr eventP);
WinHandle WinSetDrawWindow(WinHandle winHandle);
void WinSetEnabled(WinHandle const winHandle, const Boolean visible);
void WinSetVisible(WinHandle const winHandle, const Boolean visible);
