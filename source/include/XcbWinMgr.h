#pragma once

/* **** */

#include <xcb/xcb.h>

typedef xcb_connection_t* xcb_connection_p;
typedef xcb_screen_t* xcb_screen_p;

/* **** */

#include "Window.h"

/* **** */

typedef struct pxcb_window_t** pxcb_window_h;
typedef struct pxcb_window_t* pxcb_window_p;

typedef struct pxcb_window_t {
	struct {
		WinHandle window;
	}palm;
/*
	union {
		struct {
			unsigned exposed:1;
		};
	}flags;
*/
	xcb_gcontext_t background;
	xcb_gcontext_t foreground;

	pxcb_window_p nextWindow;
	xcb_window_t window;
}pxcb_window_t;

typedef struct pxcb_manager_t {
	pxcb_window_p activeWindow;
	pxcb_window_p drawWindow;
	pxcb_window_p firstWindow;
//
	xcb_connection_p connection;
	xcb_screen_p screen;
}pxcb_manager_t;

/* **** */

extern pxcb_manager_t pxcb_manager;

/* **** */

pxcb_window_p XcbCreateWindow(WinPtr const windowP, const RectangleType* windowBounds, UInt8 frameWidth);
void XcbDrawWindow_end(pxcb_window_p xwP);
pxcb_window_p XcbDrawWindow_start(WinPtr windowP);
void XcbExpose(xcb_window_t xwt);
void XcbWinDeleteWindow(WinHandle winHandle, Boolean eraseIt);
void XcbWinEraseRectangle(const RectangleType* rP, UInt16 cornerDiam);
WinHandle XcbWinGetActiveWindow(void);
WinHandle XcbWinGetDrawWindow(void);
WinPtr XcbWinGetFirstWindow(void);
WinPtr XcbWinGetNextWindow(WinPtr windowP);
void XcbWinRemoveWindow(WinHandle h2window);
WinPtr XcbWinSetActiveWindow(WinHandle winHandle);
WinPtr XcbWinSetDrawWindow(WinHandle winHandle);
