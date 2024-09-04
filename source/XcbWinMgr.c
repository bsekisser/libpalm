#include "XcbWinMgr.h"

/* **** */

#include "Window.h"

/* **** */

#include "git/libbse/include/log.h"

/* **** */

#include <assert.h>
#include <stdlib.h>
#include <string.h>

/* **** */

pxcb_manager_t pxcb_manager;

/* **** */

__attribute__((constructor))
static void __xcb_manager_globals_init(void)
{
	LOG();

	memset(&pxcb_manager, 0, sizeof(pxcb_manager_t));
}

/* **** */

static void _XcbRemoveWindow(pxcb_window_p xw, pxcb_window_p lhs)
{
	if(!xw) return;

	if(lhs)
		lhs = xw->nextWindow;
	else
		pxcb_manager.firstWindow = xw->nextWindow;
}

static WinPtr _XcbWinHandle(WinHandle const wh, pxcb_window_h h2xw, pxcb_window_h h2lhs)
{
	pxcb_window_p xw = pxcb_manager.firstWindow;

	while(xw) {
		if(wh == xw->palm.window)
			break;

		if(h2lhs) *h2lhs = xw;

		xw = xw->nextWindow;
	}

	if(h2xw)
		*h2xw = xw;

	return(xw ? wh : 0);
}

/* **** */

void XcbAddWindow(pxcb_window_p xw)
{
	xw->nextWindow = pxcb_manager.firstWindow;
	pxcb_manager.firstWindow = xw;
}

pxcb_window_p XcbCreateWindow(WinPtr const windowP, const RectangleType* windowBounds, UInt8 frameWidth)
{
	if(!pxcb_manager.connection) {
		pxcb_manager.connection = xcb_connect(NULL, NULL);

		assert(pxcb_manager.connection);

		if(!pxcb_manager.screen) {
			pxcb_manager.screen = xcb_setup_roots_iterator(
				xcb_get_setup(pxcb_manager.connection)).data;

			assert(pxcb_manager.screen);
		}
	}

	uint32_t mask;
	uint32_t values[9];

	pxcb_window_p xw = calloc(1, sizeof(pxcb_window_t));

	xw->palm.window = windowP;

	xw->foreground = xcb_generate_id(pxcb_manager.connection);
//	uint32_t mask = XCB_GC_FOREGROUND | XCB_GC_GRAPHICS_EXPOSURES;
	mask = XCB_GC_FOREGROUND;
	values[0] = pxcb_manager.screen->white_pixel;

	xcb_create_gc(pxcb_manager.connection, xw->foreground, pxcb_manager.screen->root, mask, values);

	xw->window = xcb_generate_id(pxcb_manager.connection);
	mask =
		XCB_CW_BACK_PIXEL
		| XCB_CW_SAVE_UNDER
		| XCB_CW_EVENT_MASK;
	values[0] = pxcb_manager.screen->black_pixel;
	values[1] = 1;
	values[2] =	XCB_EVENT_MASK_BUTTON_PRESS	| XCB_EVENT_MASK_KEY_RELEASE;

	xcb_create_window(pxcb_manager.connection,
		XCB_COPY_FROM_PARENT,
		xw->window,
		pxcb_manager.screen->root,
		windowBounds->topLeft.x, windowBounds->topLeft.y,
		windowBounds->extent.x, windowBounds->extent.y,
		frameWidth,
		XCB_WINDOW_CLASS_INPUT_OUTPUT,
		pxcb_manager.screen->root_visual,
		mask, &values);

	XcbAddWindow(xw);

	return(xw);
}

void XcbDrawWindow_end(pxcb_window_p xwP)
{
	xcb_flush(pxcb_manager.connection);
}

pxcb_window_p XcbDrawWindow_start(WinPtr windowP)
{
	pxcb_window_p xw = 0;

	(void)_XcbWinHandle(windowP, &xw, 0);

	xcb_map_window(pxcb_manager.connection, xw->window);

	return(xw);
}

void XcbWinDeleteWindow(WinHandle winHandle, Boolean eraseIt)
{
	pxcb_window_p xw = 0, lhs = 0;

	(void)_XcbWinHandle(winHandle, &xw, &lhs);

	if(eraseIt)
		LOG("TODO: eraseIt");

	xcb_unmap_window(pxcb_manager.connection, xw->window);

	_XcbRemoveWindow(xw, lhs);

	free(xw);
}

WinHandle XcbWinGetActiveWindow(void)
{
	pxcb_window_p xw = pxcb_manager.activeWindow;

	return(xw ? xw->palm.window : 0);
}

WinHandle XcbWinGetDrawWindow(void)
{
	pxcb_window_p xw = pxcb_manager.drawWindow;

	return(xw ? xw->palm.window : 0);
}

WinPtr XcbWinGetFirstWindow(void)
{
	pxcb_window_p xw = pxcb_manager.firstWindow;

	return(xw ? xw->palm.window : 0);
}


WinPtr XcbWinGetNextWindow(WinPtr windowP)
{
	pxcb_window_p xw = 0;

	(void)_XcbWinHandle(windowP, &xw, 0);

	pxcb_window_p nextWindow = xw ? xw->nextWindow : 0;

	return(nextWindow ? nextWindow->palm.window : 0);
}

void XcbWinRemoveWindow(WinHandle h2window)
{
	pxcb_window_p xw = 0, lhs = 0;

	(void)_XcbWinHandle(h2window, &xw, &lhs);

	_XcbRemoveWindow(xw, lhs);
}


WinPtr XcbWinSetActiveWindow(WinHandle winHandle)
{
	WinPtr activeWindow = XcbWinGetActiveWindow();

	(void)_XcbWinHandle(winHandle, &pxcb_manager.activeWindow, 0);

	return(activeWindow);
}

WinPtr XcbWinSetDrawWindow(WinHandle winHandle)
{
	WinPtr drawWindow = XcbWinGetDrawWindow();

	(void)_XcbWinHandle(winHandle, &pxcb_manager.drawWindow, 0);

	return(drawWindow);
}
