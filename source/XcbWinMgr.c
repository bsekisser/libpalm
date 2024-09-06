#include "config.h"

#include "XcbWinMgr.h"

/* **** */

#include "Window.h"
#include "xRect.h"

/* **** */

#include "git/libbse/include/err_test.h"
#include "git/libbse/include/log.h"

/* **** */

#include <assert.h>
#include <inttypes.h>
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
	PEDANTIC(assert(xw));

	if(!xw) return;

	if(lhs)
		lhs->nextWindow = xw->nextWindow;
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
	PEDANTIC(assert(xw));

	xw->nextWindow = pxcb_manager.firstWindow;
	pxcb_manager.firstWindow = xw;
}

pxcb_window_p XcbCreateWindow(WinPtr const windowP, const RectangleType* windowBounds, UInt8 frameWidth)
{
	PEDANTIC(assert(windowP));
	PEDANTIC(assert(windowBounds));

	if(!pxcb_manager.connection) {
		pxcb_manager.connection = xcb_connect(NULL, NULL);

		PEDANTIC(assert(pxcb_manager.connection));

		if(!pxcb_manager.connection) return(0);

		if(!pxcb_manager.screen) {
			pxcb_manager.screen = xcb_setup_roots_iterator(
				xcb_get_setup(pxcb_manager.connection)).data;

			PEDANTIC(assert(pxcb_manager.screen));
		}
	}

	uint32_t mask;
	uint32_t values[9], *valueP = values;

	pxcb_window_p xw = calloc(1, sizeof(pxcb_window_t));
	ERR_NULL(xw);

	xw->palm.window = windowP;

/*
	xw->gc = xcb_generate_id(pxcb_manager.connection);
//	uint32_t mask = XCB_GC_FOREGROUND | XCB_GC_GRAPHICS_EXPOSURES;
	mask = XCB_GC_FOREGROUND | XCB_GC_BACKGROUND;
	valueP = values;
	*valueP++ = pxcb_manager.screen->white_pixel;
	*valueP++ = pxcb_manager.screen->black_pixel;

	xcb_create_gc(pxcb_manager.connection, xw->gc, pxcb_manager.screen->root, mask, values);
*/

	xw->window = xcb_generate_id(pxcb_manager.connection);
	mask =
		XCB_CW_BACK_PIXEL
		| XCB_CW_SAVE_UNDER
		| XCB_CW_EVENT_MASK;
	valueP = values;
	*valueP++ = pxcb_manager.screen->white_pixel;
	*valueP++ = 1;
	*valueP++ =	XCB_EVENT_MASK_BUTTON_PRESS	| XCB_EVENT_MASK_KEY_RELEASE;

	LOG_RECTANGLE(windowBounds);

	xcb_create_window(pxcb_manager.connection,
		XCB_COPY_FROM_PARENT,
		xw->window,
		pxcb_manager.screen->root,
		windowBounds->topLeft.x, windowBounds->topLeft.y,
		windowBounds->extent.x, windowBounds->extent.y,
		frameWidth,
		XCB_WINDOW_CLASS_INPUT_OUTPUT,
		0,
//		pxcb_manager.screen->root_visual,
		mask, &values);

	XcbAddWindow(xw);

	return(xw);
}

void XcbDrawWindow_end(pxcb_window_p xwP)
{
	PEDANTIC(assert(xwP));

	if(pxcb_manager.connection)
		xcb_flush(pxcb_manager.connection);
}

pxcb_window_p XcbDrawWindow_start(WinPtr windowP)
{
	PEDANTIC(assert(windowP));

	pxcb_window_p xw = 0;

	(void)_XcbWinHandle(windowP, &xw, 0);

	if(pxcb_manager.connection)
		xcb_map_window(pxcb_manager.connection, xw->window);

	return(xw);
}

void XcbWinDeleteWindow(WinHandle winHandle, Boolean eraseIt)
{
	PEDANTIC(assert(winHandle));

	pxcb_window_p xw = 0, lhs = 0;

	(void)_XcbWinHandle(winHandle, &xw, &lhs);

	if(eraseIt)
		LOG("TODO: eraseIt");

	if(pxcb_manager.connection)
		xcb_unmap_window(pxcb_manager.connection, xw->window);

	_XcbRemoveWindow(xw, lhs);

	free(xw);
}

void XcbWinEraseRectangle(const RectangleType* rP, UInt16 cornerDiam)
{
	PEDANTIC(assert(rP));

	xcb_void_cookie_t cookie;
	xcb_generic_error_t* error;

	if(cornerDiam)
		LOG("TODO: corerDiam: %u", cornerDiam);

	LOG_RECTANGLE(rP);

	xcb_connection_p connection = pxcb_manager.connection;
	xcb_window_t drawWindow = pxcb_manager.drawWindow->window;

	if(0) {
		LOG_START("connection: 0x%016" PRIxPTR, (uintptr_t)connection);
		_LOG_(", drawWindow: 0x%016" PRIxPTR, (uintptr_t)pxcb_manager.drawWindow);
		LOG_END("(0x%08x)", drawWindow);
	}

	xcb_gcontext_t gc = xcb_generate_id(connection);

	uint32_t mask = XCB_GC_BACKGROUND;
	uint32_t values = pxcb_manager.screen->white_pixel;

	xcb_create_gc(connection, gc, drawWindow, mask, &values);

	xcb_rectangle_t xr = {
		rP->topLeft.x, rP->topLeft.y,
		rP->extent.x, rP->extent.y };

	cookie = xcb_poly_fill_rectangle_checked(connection, drawWindow,
		gc, 1, &xr);

	if((error = xcb_request_check(connection, cookie))) {
		LOG("poly_rectangle_failed");
		exit(-1);
	}

	xcb_flush(connection);
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
