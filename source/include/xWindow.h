#pragma once

/* **** */

#include "config.h"

/* **** */

//#define ALLOW_ACCESS_TO_INTERNALS_OF_WINDOWS

#include "sdk/include/Core/System/Window.h"

/* **** */

typedef struct window_manager_t* window_manager_p;

/* **** */

#ifndef ALLOW_ACCESS_TO_INTERNALS_OF_WINDOWS
	#include <xcb/xcb.h>

	typedef xcb_connection_t* xcb_connection_p;
	typedef xcb_screen_t* xcb_screen_p;
	typedef xcb_window_t* xcb_window_p;

	typedef struct WindowFlagsType {
		unsigned dialog:1;
		unsigned enabled:1;
		unsigned focusable:1;
		unsigned modal:1;
	}WindowFlagsType;

	typedef struct WindowType {
		WindowFlagsType windowFlags;
		RectangleType windowBounds;
		AbsRectType clippingBounds;
		FrameBitsType frameType;
		WinHandle nextWindow;
		struct {
			xcb_window_t window;
		}xcb;
	}WindowType;
#endif

typedef struct window_manager_t {
	WinHandle activeWindow;
	WinHandle drawWindow;
	WinHandle enterWindowID;
	WinHandle exitWindowID;
	WinHandle exitedWindowID;
	WinHandle firstWindow;
	struct {
		xcb_connection_p connection;
		xcb_screen_p screen;
	}xcb;
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
WinPtr WinGetNextWindow(WinPtr windowP);
WinHandle WinSetDrawWindow(WinHandle winHandle);
