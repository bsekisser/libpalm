#include "config.h"

#include "XcbWinMgr.h"

/* **** */

#include "Window.h"
#include "xRect.h"

/* **** */

#include "git/libbse/include/err_test.h"
#include "git/libbse/include/log.h"
#include "git/libbse/include/unused.h"

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
	AT_INIT(LOG());

	memset(&pxcb_manager, 0, sizeof(pxcb_manager_t));
}

/* **** */

#define XCB_LOG_POINT(_r) _xcb_log_point(_r, # _r, __FUNCTION__, __LINE__)
void _xcb_log_point(xcb_point_ref r, const char* name, const char* func, const unsigned line)
{
	printf("%s:%04u: ", func, line);

	LOG_END("%s->(x: %d, y: %d)",
		name, r->x, r->y);
}

#define XCB_LOG_RECTANGLE(_r) _xcb_log_rectangle(_r, # _r, __FUNCTION__, __LINE__)
void _xcb_log_rectangle(xcb_rectangle_ref r, const char* name, const char* func, const unsigned line)
{
	printf("%s:%04u: ", func, line);

	LOG_END("%s->(x: %d, y: %d, width: %u, height: %u)",
		name, r->x, r->y, r->width, r->height);
}

static WinHandle _XcbPalmWindow(pxcb_window_p xw)
{ return(xw ? xw->palm.window : 0); }

static void _XcbRemoveWindow(pxcb_window_p xw, pxcb_window_p lhs)
{
	PEDANTIC(assert(xw));

	if(!xw) return;

	pxcb_window_p const nextWindow = xw->nextWindow;
	xw->nextWindow = 0;

	if(lhs)
		lhs->nextWindow = nextWindow;
	else
		pxcb_manager.firstWindow = nextWindow;
}

static WinPtr _XcbWinHandle(WinHandle const wh, pxcb_window_h h2xw, pxcb_window_h h2lhs)
{
	PEDANTIC(assert(wh));

	if(!wh) return(0);

	pxcb_window_p lhs, xw = pxcb_manager.firstWindow;

	while(xw) {
		if(wh == xw->palm.window)
			break;

		lhs = xw;
		xw = lhs->nextWindow;
	}

	if(h2lhs)
		*h2lhs = xw;

	if(h2xw)
		*h2xw = xw;

	return(xw ? wh : 0);
}

/* **** */

void XcbAddWindow(pxcb_window_p xw)
{
	PEDANTIC(assert(xw));

	if(!xw) return;

	xw->nextWindow = pxcb_manager.firstWindow;
	pxcb_manager.firstWindow = xw;
}

pxcb_window_p XcbCreateWindow(WinPtr const windowP, const RectangleType* windowBounds, UInt8 frameWidth)
{
	PEDANTIC(assert(windowP));
	PEDANTIC(assert(windowBounds));

	if(!(windowP && windowBounds)) return(0);

	if(!pxcb_manager.connection) {
		pxcb_manager.connection = xcb_connect(NULL, NULL);

		PEDANTIC(assert(pxcb_manager.connection));

		if(!pxcb_manager.connection) return(0);
	}

	if(!pxcb_manager.screen) {
		pxcb_manager.screen = xcb_setup_roots_iterator(
			xcb_get_setup(pxcb_manager.connection)).data;

		PEDANTIC(assert(pxcb_manager.screen));

		LOGu(pxcb_manager.screen->width_in_pixels);
		LOGu(pxcb_manager.screen->height_in_pixels);

		if(!pxcb_manager.screen) return(0);
	}

	xcb_connection_p connection = pxcb_manager.connection;
	xcb_void_cookie_t cookie;
	xcb_generic_error_p error = 0;
	xcb_screen_p screen = pxcb_manager.screen;

	uint32_t mask;
	uint32_t values[9], *valueP = values;

	pxcb_window_p xw = calloc(1, sizeof(pxcb_window_t));
	ERR_NULL(xw);

	xw->palm.window = windowP;

	/* **** create the x window */

	xw->window = xcb_generate_id(connection);
	mask =
		XCB_CW_BACK_PIXEL |
		XCB_CW_SAVE_UNDER |
		XCB_CW_EVENT_MASK;
	valueP = values;
	*valueP++ = screen->white_pixel;
	*valueP++ = 1;
	*valueP++ =
		XCB_EVENT_MASK_BUTTON_PRESS |
		XCB_EVENT_MASK_KEY_RELEASE;

	LOG_RECTANGLE(windowBounds);

	xw->scale.x = (screen->width_in_pixels / (windowBounds->extent.x - windowBounds->topLeft.x)) ?: 1;
	xw->scale.y = (screen->height_in_pixels / (windowBounds->extent.y - windowBounds->topLeft.y)) ?: 1;

	XCB_LOG_POINT(&xw->scale);

	xcb_rectangle_t scaledBounds;
	XcbWin2XcbScaledRectangle(windowBounds, &scaledBounds, &xw->scale);

	XCB_LOG_RECTANGLE(&scaledBounds);

	cookie = xcb_create_window_checked(connection,
		screen->root_depth,
//		XCB_COPY_FROM_PARENT,
		xw->window,
		screen->root,
		scaledBounds.x, scaledBounds.y,
		scaledBounds.width, scaledBounds.height,
		frameWidth,
		XCB_WINDOW_CLASS_INPUT_OUTPUT,
		screen->root_visual,
		mask, &values);

	if((error = xcb_request_check(connection, cookie))) {
		LOG("unable to create window: %d", error->error_code);
		xcb_disconnect(connection);
		exit(-1);
	}

	/* **** background */

	xw->background = xcb_generate_id(connection);
	mask = XCB_GC_BACKGROUND;
	valueP = values;
	*valueP++ = screen->white_pixel;

	cookie = xcb_create_gc_checked(connection, xw->background, xw->window, mask, values);

	if((error = xcb_request_check(connection, cookie))) {
		LOG("unable to create background graphic context: %d", error->error_code);
		xcb_disconnect(connection);
		exit(-1);
	}

	/* **** foreground */

	xw->foreground = xcb_generate_id(connection);
	mask = XCB_GC_FOREGROUND;
	valueP = values;
	*valueP++ = screen->black_pixel;

	cookie = xcb_create_gc_checked(connection, xw->foreground, xw->window, mask, values);

	if((error = xcb_request_check(connection, cookie))) {
		LOG("unable to create foreground graphic context: %d", error->error_code);
		xcb_disconnect(connection);
		exit(-1);
	}

	/* **** */

	xcb_flush(connection);

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

	if(!windowP) return(0);

	xcb_connection_p connection = pxcb_manager.connection;
	pxcb_window_p xw = 0;

	(void)_XcbWinHandle(windowP, &xw, 0);

	xcb_void_cookie_t cookie = xcb_map_window_checked(connection, xw->window);

	xcb_generic_error_p error = 0;

	if((error = xcb_request_check(connection, cookie))) {
		LOG("unable to map window: %d", error->error_code);
		xcb_disconnect(connection);
		exit(-1);
	}

	return(xw);
}

void XcbScaleRectangle(xcb_rectangle_ref xr, xcb_point_ref scaleP)
{
	xcb_point_ref scale = scaleP ?: &pxcb_manager.drawWindow->scale;

	XCB_LOG_POINT(scale);
	XCB_LOG_RECTANGLE(xr);

	xr->x *= scale->x;
	xr->y *= scale->y;
	xr->width *= scale->x;
	xr->height *= scale->y;

	XCB_LOG_RECTANGLE(xr);
}

void XcbWin2XcbRectandle(const RectangleType *const wr, xcb_rectangle_ref xr)
{
	LOG_RECTANGLE(wr);

	xr->x = wr->topLeft.x;
	xr->y = wr->topLeft.y;
	xr->width = wr->extent.x;
	xr->height = wr->extent.y;

	XCB_LOG_RECTANGLE(xr);
}

void XcbWin2XcbScaledRectangle(const RectangleType *const wr, xcb_rectangle_ref xr, xcb_point_ref scale)
{
	XcbWin2XcbRectandle(wr, xr);
	XcbScaleRectangle(xr, scale);
}

void XcbWinDeleteWindow(WinHandle winHandle, Boolean eraseIt)
{
	PEDANTIC(assert(winHandle));

	if(!winHandle) return;

	pxcb_window_p xw = 0, lhs = 0;

	(void)_XcbWinHandle(winHandle, &xw, &lhs);

	if(eraseIt)
		LOG("TODO: eraseIt");

	xcb_connection_p connection = pxcb_manager.connection;

	if(!connection) goto failConnectionExit;

	xcb_void_cookie_t cookie;
	xcb_generic_error_p error = 0;

	if(xw->foreground) {
		cookie = xcb_free_gc_checked(connection, xw->foreground);

		if((error = xcb_request_check(connection, cookie))) {
			LOG("unable to free foreground context: %d", error->error_code);
			xcb_disconnect(connection);
			exit(-1);
		}

		PEDANTIC(xw->foreground = 0);
	}

	if(xw->background) {
		cookie = xcb_free_gc_checked(connection, xw->background);

		if((error = xcb_request_check(connection, cookie))) {
			LOG("unable to free background context: %d", error->error_code);
			xcb_disconnect(connection);
			exit(-1);
		}

		PEDANTIC(xw->background = 0);
	}

	if(xw->window) {
		cookie = xcb_unmap_window_checked(pxcb_manager.connection, xw->window);

		if((error = xcb_request_check(connection, cookie))) {
			LOG("unable to unmap window: %d", error->error_code);
			xcb_disconnect(connection);
			exit(-1);
		}

		PEDANTIC(xw->window = 0);
	}

	xcb_flush(connection);

failConnectionExit:
	_XcbRemoveWindow(xw, lhs);

	free(xw);
}

void XcbWinEraseRectangle(const RectangleType* rP, UInt16 cornerDiam)
{
	PEDANTIC(assert(rP));

	xcb_void_cookie_t cookie;
	xcb_generic_error_p error;

	if(cornerDiam)
		LOG("TODO: corerDiam: %u", cornerDiam);

	LOG_RECTANGLE(rP);

	xcb_connection_p connection = pxcb_manager.connection;
	pxcb_window_p drawWindow = pxcb_manager.drawWindow;
	xcb_window_t xDrawWindow = drawWindow->window;

	PEDANTIC(assert(connection));
	PEDANTIC(assert(drawWindow));
	PEDANTIC(assert(xDrawWindow));
	PEDANTIC(assert(drawWindow->background));

	if(!(connection && drawWindow)) return;

	if(0) {
		LOG_START("connection: 0x%016" PRIxPTR, (uintptr_t)connection);
		_LOG_(", drawWindow: 0x%016" PRIxPTR, (uintptr_t)drawWindow);
		LOG_END("(0x%08x)", xDrawWindow);
	}

	xcb_rectangle_t xr;

	XcbWin2XcbScaledRectangle(rP, &xr, 0);

	xcb_gcontext_t gc = xcb_generate_id(connection);
	uint32_t mask = XCB_GC_BACKGROUND;
	uint32_t values = pxcb_manager.screen->white_pixel;

	cookie = xcb_create_gc_checked(connection, gc, xDrawWindow, mask, &values);

	if((error = xcb_request_check(connection, cookie))) {
		LOG("create gcontext failed: %d", error->error_code);
		xcb_disconnect(connection);
		exit(-1);
	}

	cookie = xcb_poly_rectangle_checked(connection, xDrawWindow,
		drawWindow->foreground, 1, &xr);

	if((error = xcb_request_check(connection, cookie))) {
		LOG("poly_rectangle_failed: %d", error->error_code);
		xcb_disconnect(connection);
		exit(-1);
	}

	xcb_flush(connection);
}

WinHandle XcbWinGetActiveWindow(void)
{ return(_XcbPalmWindow(pxcb_manager.activeWindow)); }

WinHandle XcbWinGetDrawWindow(void)
{ return(_XcbPalmWindow(pxcb_manager.drawWindow)); }

WinPtr XcbWinGetFirstWindow(void)
{ return(_XcbPalmWindow(pxcb_manager.firstWindow)); }

WinPtr XcbWinGetNextWindow(WinPtr windowP)
{
	PEDANTIC(assert(windowP));

	pxcb_window_p xw = 0;

	(void)_XcbWinHandle(windowP, &xw, 0);

	pxcb_window_p nextWindow = xw ? xw->nextWindow : 0;

	return(_XcbPalmWindow(nextWindow));
}

void XcbWinRemoveWindow(WinHandle h2window)
{
	pxcb_window_p xw = 0, lhs = 0;

	(void)_XcbWinHandle(h2window, &xw, &lhs);

	_XcbRemoveWindow(xw, lhs);
}

void XcbWinScalePoint(PointType *const pointP, Boolean ceiling)
{
	xcb_point_ref scale = &pxcb_manager.drawWindow->scale;

	pointP->x *= scale->x;
	pointP->y *= scale->y;

	UNUSED(ceiling);
}

void XcbWinScaleRectangle(RectangleType *const rectP)
{
	XcbWinScalePoint(&rectP->topLeft, 0);
	XcbWinScalePoint(&rectP->extent, 0);
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
