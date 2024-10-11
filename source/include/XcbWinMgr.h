#pragma once

/* **** */

#include <xcb/xcb.h>

typedef xcb_connection_t* xcb_connection_p;
typedef xcb_generic_error_t* xcb_generic_error_p;
typedef xcb_point_t *const xcb_point_ref;
typedef xcb_rectangle_t *const xcb_rectangle_ref;
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

	xcb_point_t scale;

	xcb_gcontext_t foreground;
	xcb_gcontext_t background;

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
void XcbScaleRectangle(xcb_rectangle_ref xr, xcb_point_ref scaleP);
void XcbWin2XcbScaledRectangle(const RectangleType *const wr, xcb_rectangle_ref xr, xcb_point_ref scale);
void XcbWinDeleteWindow(WinHandle winHandle, Boolean eraseIt);
void XcbWinEraseRectangle(const RectangleType* rP, UInt16 cornerDiam);
WinHandle XcbWinGetActiveWindow(void);
WinHandle XcbWinGetDrawWindow(void);
WinPtr XcbWinGetFirstWindow(void);
WinPtr XcbWinGetNextWindow(WinPtr windowP);
void XcbWinRemoveWindow(WinHandle h2window);
void XcbWinScalePoint(PointType* pointP, Boolean ceiling);
void XcbWinScaleRectangle(RectangleType* rectP);
WinPtr XcbWinSetActiveWindow(WinHandle winHandle);
WinPtr XcbWinSetDrawWindow(WinHandle winHandle);
