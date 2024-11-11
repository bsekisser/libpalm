#include "config.h"

#include "XcbWinMgr.h"

/* **** */

#include "xWindow.h"
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

struct config_xcb_window_t {
	struct {
		struct {
			unsigned entry:1;
			unsigned exit:1;
		}at;
	}trace;
}xcb_window_config;

#define TRACE xcb_window_config.trace

/* **** */

__attribute__((constructor))
static void __xcb_window_config_init(void)
{
	AT_INIT(LOG());

	memset(&xcb_window_config, 0, sizeof(xcb_window_config));

	xcb_window_config.trace.at.entry = 1;
	xcb_window_config.trace.at.exit = 1;
}

__attribute__((constructor))
static void __xcb_window_manager_globals_init(void)
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

static void _XcbMapWindow(pxcb_window_ref xw)
{
	TRACE_ENTRY();

	PEDANTIC(assert(xw));

	if(!xw) LOG_ACTION(return);
	if(!xw->window) LOG_ACTION(return);
	if(xw->flags.map.called) LOG_ACTION(return);

	xcb_connection_p connection = pxcb_manager.connection;
	if(!pxcb_manager.connection)
		LOG_ACTION(return);

	xw->flags.map.called = 1;
	xw->flags.unmap.raw = 0;

	xcb_void_cookie_t cookie = xcb_map_window_checked(connection, xw->window);

	xcb_generic_error_p error = 0;

	if((error = xcb_request_check(connection, cookie))) {
		LOG("unable to map window: %d", error->error_code);
		xcb_disconnect(connection);
		exit(-1);
	}

	xcb_flush(connection);

	TRACE_EXIT();
}

static WinHandle _XcbPalmWindow(pxcb_window_p xw)
{ return(xw ? xw->palm.window : 0); }

static void _XcbRemoveWindow(pxcb_window_p xw, pxcb_window_p lhs)
{
	PEDANTIC(assert(xw));

	if(!xw) return;

	pxcb_window_ref nextWindow = xw->nextWindow;
//	xcb_window_t nxwt = nextWindow ? nextWindow->window : 0;
	WinHandle nxwt = _XcbPalmWindow(nextWindow);

	xw->nextWindow = 0;

	if(lhs)
		lhs->nextWindow = nextWindow;
	else
		pxcb_manager.firstWindow = nextWindow;

	if(pxcb_manager.activeWindow == xw) {
		if(0) {
			LOG_START("nextWindow: 0x%016" PRIxPTR, (uintptr_t)nextWindow);
			LOG_END("(0x%016" PRIxPTR ")", (uintptr_t)nxwt);
		}

		pxcb_manager.activeWindow = nextWindow;
	}
	if(pxcb_manager.drawWindow == xw) {
		if(0) {
			LOG_START("nextWindow: 0x%016" PRIxPTR, (uintptr_t)nextWindow);
			LOG_END("(0x%016" PRIxPTR ")", (uintptr_t)nxwt);
		}

		pxcb_manager.drawWindow = nextWindow;
	}
}

static void _XcbUnmapWindow(pxcb_window_ref xw)
{
	LOG();

	PEDANTIC(assert(xw));

	if(!xw) LOG_ACTION(return);
	if(!xw->window) LOG_ACTION(return);
	if(xw->flags.unmap.called) LOG_ACTION(return);

	xcb_connection_p connection = pxcb_manager.connection;
	if(!pxcb_manager.connection)
		LOG_ACTION(return);

	xw->flags.unmap.called = 1;

	xcb_void_cookie_t cookie = xcb_unmap_window_checked(connection, xw->window);

	xcb_generic_error_p error = 0;

	if((error = xcb_request_check(connection, cookie))) {
		LOG("unable to map window: %d", error->error_code);
		xcb_disconnect(connection);
		exit(-1);
	}

	xcb_flush(connection);
}

static pxcb_window_p _XcbWindow(const xcb_window_t xwt, pxcb_window_href h2lhs)
{
	PEDANTIC(assert(xwt));

	if(!xwt) return(0);

	pxcb_window_p lhs = 0, xw = pxcb_manager.firstWindow;

	while(xw) {
		if(xwt == xw->window)
			break;

		lhs = xw;
		xw = xw->nextWindow;
	}

	if(h2lhs)
		*h2lhs = lhs;

	return(xw);
}

static WinPtr _XcbWinHandle(WinHandle const wh, pxcb_window_h h2xw, pxcb_window_h h2lhs)
{
	if(!wh) return(0);

	pxcb_window_p lhs = 0, xw = pxcb_manager.firstWindow;

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
		XCB_EVENT_MASK_EXPOSURE |
		XCB_EVENT_MASK_KEY_RELEASE;

//	LOG_RECTANGLE(windowBounds);

	xw->scale.x = (screen->width_in_pixels / (windowBounds->extent.x - windowBounds->topLeft.x)) / 2 ?: 1;
	xw->scale.y = (screen->height_in_pixels / (windowBounds->extent.y - windowBounds->topLeft.y)) / 2 ?: 1;

//	XCB_LOG_POINT(&xw->scale);

	xcb_rectangle_t scaledBounds;
	XcbWin2XcbScaledRectangle(windowBounds, &scaledBounds, &xw->scale);

//	XCB_LOG_RECTANGLE(&scaledBounds);

	cookie = xcb_create_window_checked(connection,
		screen->root_depth,
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

	/* **** background >>>> foreground erase */

	xw->background = xcb_generate_id(connection);
	mask = XCB_GC_FOREGROUND;
//		XCB_GC_GRAPHICS_EXPOSURES;
	valueP = values;
	*valueP++ = screen->white_pixel;
//	*valueP++ = 1;

	cookie = xcb_create_gc_checked(connection, xw->background, xw->window, mask, values);

	if((error = xcb_request_check(connection, cookie))) {
		LOG("unable to create background graphic context: %d", error->error_code);
		xcb_disconnect(connection);
		exit(-1);
	}

	/* **** foreground */

	const char* font_name = "variable";

	xcb_font_t font = xcb_generate_id(connection);
	cookie = xcb_open_font_checked(connection, font,
		strlen(font_name), font_name);

	if((error = xcb_request_check(connection, cookie))) {
		LOG_ERR("unable to open font (%s): %d", font_name, error->error_code);
		xcb_disconnect(connection);
		exit(-1);
	}

	xw->foreground = xcb_generate_id(connection);
	mask = XCB_GC_FOREGROUND
		| XCB_GC_BACKGROUND
		| XCB_GC_FONT;
	valueP = values;
	*valueP++ = screen->black_pixel;
	*valueP++ = screen->white_pixel;
	*valueP++ = font;

	cookie = xcb_create_gc_checked(connection, xw->foreground, xw->window, mask, values);

	if((error = xcb_request_check(connection, cookie))) {
		LOG("unable to create foreground graphic context: %d", error->error_code);
		xcb_disconnect(connection);
		exit(-1);
	}

	cookie = xcb_close_font_checked(connection, font);

	if((error = xcb_request_check(connection, cookie))) {
		LOG_ERR("unable to close font(%s): %d", font_name, error->error_code);
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

	pxcb_window_p xw = 0;

	(void)_XcbWinHandle(windowP, &xw, 0);

	return(xw);
}

int XcbExposeEvent(const xcb_expose_event_t* ev)
{
	PEDANTIC(assert(ev));

	TRACE_ENTRY("window: 0x%08x", ev->window);

	pxcb_window_ref xw = _XcbWindow(ev->window, 0);

	xw->flags.exposed = 1;
	xw->flags.unmap.raw = 0;

	TRACE_EXIT();

	return(1);
}

void XcbScalePoint(xcb_point_ref xp, xcb_point_ref scaleP)
{
	xcb_point_ref scale = scaleP ?: &pxcb_manager.drawWindow->scale;

	xp->x *= scale->x ?: 1;
	xp->y *= scale->y ?: 1;
}

void XcbScaleRectangle(xcb_rectangle_ref xr, xcb_point_ref scaleP)
{
	xcb_point_ref scale = scaleP ?: &pxcb_manager.drawWindow->scale;

//	XCB_LOG_POINT(scale);
//	XCB_LOG_RECTANGLE(xr);

	xr->x *= scale->x ?: 1;
	xr->y *= scale->y ?: 1;
	xr->width *= scale->x ?: 1;
	xr->height *= scale->y ?: 1;

//	XCB_LOG_RECTANGLE(xr);
}

int XcbUnmapNotifyEvent(const xcb_unmap_notify_event_t* ev)
{
	TRACE_ENTRY();

	pxcb_window_ref xw = _XcbWindow(ev->window, 0);

	xw->flags.exposed = 0;
	xw->flags.unmap.done = 1;

	TRACE_EXIT();

	return(1);
}

void XcbWin2XcbRectandle(const RectangleType *const wr, xcb_rectangle_ref xr)
{
//	LOG_RECTANGLE(wr);

	xr->x = wr->topLeft.x;
	xr->y = wr->topLeft.y;
	xr->width = wr->extent.x;
	xr->height = wr->extent.y;

//	XCB_LOG_RECTANGLE(xr);
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

void XcbWinDrawChars(const char* chars, size_t len, const unsigned x, const unsigned y)
{
	TRACE_ENTRY();

	xcb_connection_p connection = pxcb_manager.connection;
	pxcb_window_ref drawWindow = pxcb_manager.drawWindow;

	if(!connection) return;
	if(!drawWindow) return;

	PEDANTIC(assert(drawWindow->foreground));

	xcb_point_t xpt = { .x = x, .y = y};
	XcbScalePoint(&xpt, 0);

	xcb_void_cookie_t cookie = xcb_image_text_8_checked(connection,
		len, drawWindow->window, drawWindow->foreground, xpt.x, xpt.y, chars);

	xcb_generic_error_p error = 0;
	if((error = xcb_request_check(connection, cookie))) {
		LOG("call to xcb_image_text_8 failed: %d", error->error_code);
		xcb_disconnect(connection);
		exit(-1);
	}

	xcb_flush(connection);

	TRACE_EXIT();
}

void XcbWinDrawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, WinDrawOperation mode)
{
	xcb_void_cookie_t cookie;
	xcb_generic_error_p error;

	if(1) {
		LOG("left: %u, top: %u, right: %u, bottom: %u",
			x1, y1, x2, y2);
	}

	xcb_connection_p connection = pxcb_manager.connection;
	if(!pxcb_manager.connection)
		LOG_ACTION(return);

	pxcb_window_p drawWindow = pxcb_manager.drawWindow;
	if(!drawWindow) LOG_ACTION(return);

	xcb_window_t xDrawWindow = drawWindow->window;

	if(0) {
		LOG_START("connection: 0x%016" PRIxPTR, (uintptr_t)connection);
		_LOG_(", drawWindow: 0x%016" PRIxPTR, (uintptr_t)drawWindow);
		LOG_END("(0x%08x)", xDrawWindow);
	}

	PEDANTIC(assert(connection));
	PEDANTIC(assert(drawWindow));
	PEDANTIC(assert(xDrawWindow));
	PEDANTIC(assert(drawWindow->background));
	PEDANTIC(assert(drawWindow->foreground));
	PEDANTIC(assert(drawWindow->flags.exposed));

	if(!(connection && drawWindow)) return;

	if(0) {
		LOG_START("connection: 0x%016" PRIxPTR, (uintptr_t)connection);
		_LOG_(", drawWindow: 0x%016" PRIxPTR, (uintptr_t)drawWindow);
		LOG_END("(0x%08x)", xDrawWindow);
	}

	xcb_point_t polyLine[2] = {
		{ .x = x1, .y = y1 },
		{ .x = x2, .y = y2 },
	};

	XcbScalePoint(&polyLine[0], 0);
	XcbScalePoint(&polyLine[1], 0);

	switch(mode) {
		case winErase:
			cookie = xcb_poly_line_checked(connection,
				XCB_COORD_MODE_ORIGIN,
				xDrawWindow, drawWindow->background, 2, polyLine);
			break;
		case winPaint:
			cookie = xcb_poly_line_checked(connection,
				XCB_COORD_MODE_ORIGIN,
				xDrawWindow, drawWindow->foreground, 2, polyLine);
			break;
		default:
			LOG("unhandled draw mode: %u", mode);
			break;
	}

	if((error = xcb_request_check(connection, cookie))) {
		LOG("poly_rectangle_failed: %d", error->error_code);
		xcb_disconnect(connection);
		exit(-1);
	}

	xcb_flush(connection);
}

void XcbWinDrawRectangle(const RectangleType* rP, UInt16 cornerDiam, WinDrawOperation mode)
{
	PEDANTIC(assert(rP));

	xcb_void_cookie_t cookie;
	xcb_generic_error_p error;

	if(cornerDiam)
		LOG("TODO: corerDiam: %u", cornerDiam);

	LOG_RECTANGLE(rP);

	xcb_connection_p connection = pxcb_manager.connection;
	if(!pxcb_manager.connection)
		LOG_ACTION(return);

	pxcb_window_p drawWindow = pxcb_manager.drawWindow;
	if(!drawWindow) LOG_ACTION(return);

	xcb_window_t xDrawWindow = drawWindow->window;

	if(0) {
		LOG_START("connection: 0x%016" PRIxPTR, (uintptr_t)connection);
		_LOG_(", drawWindow: 0x%016" PRIxPTR, (uintptr_t)drawWindow);
		LOG_END("(0x%08x)", xDrawWindow);
	}

	PEDANTIC(assert(connection));
	PEDANTIC(assert(drawWindow));
	PEDANTIC(assert(xDrawWindow));
	PEDANTIC(assert(drawWindow->background));
	PEDANTIC(assert(drawWindow->foreground));
	PEDANTIC(assert(drawWindow->flags.exposed));

	if(!(connection && drawWindow)) return;

	if(0) {
		LOG_START("connection: 0x%016" PRIxPTR, (uintptr_t)connection);
		_LOG_(", drawWindow: 0x%016" PRIxPTR, (uintptr_t)drawWindow);
		LOG_END("(0x%08x)", xDrawWindow);
	}

	xcb_rectangle_t xr;

	XcbWin2XcbScaledRectangle(rP, &xr, 0);

	switch(mode) {
		case winErase:
			cookie = xcb_poly_fill_rectangle_checked(connection, xDrawWindow,
				drawWindow->background, 1, &xr);
			break;
		case winPaint:
			cookie = xcb_poly_rectangle_checked(connection, xDrawWindow,
				drawWindow->foreground, 1, &xr);
			break;
		default:
			LOG("unhandled draw mode: %u", mode);
			break;
	}

	if((error = xcb_request_check(connection, cookie))) {
		LOG("poly_rectangle_failed: %d", error->error_code);
		xcb_disconnect(connection);
		exit(-1);
	}

	xcb_flush(connection);
}

int XcbWinEnterWindow(WinHandle const enterWindow, WinHandle const exitWindow)
{
	if(0) {
		LOG_START("enterWindow: 0x%016" PRIxPTR, (uintptr_t)enterWindow);
		LOG_END(", exitWindow: 0x%016" PRIxPTR, (uintptr_t)exitWindow);
	}

	if(XcbWinUnmapWindow(exitWindow))
		return(XcbWinMapWindow(enterWindow));

	return(0);
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
	if(!pxcb_manager.connection)
		LOG_ACTION(return);

	pxcb_window_p drawWindow = pxcb_manager.drawWindow;
	if(!drawWindow) LOG_ACTION(return);

	xcb_window_t xDrawWindow = drawWindow->window;

	if(0) {
		LOG_START("connection: 0x%016" PRIxPTR, (uintptr_t)connection);
		_LOG_(", drawWindow: 0x%016" PRIxPTR, (uintptr_t)drawWindow);
		LOG_END("(0x%08x)", xDrawWindow);
	}

	PEDANTIC(assert(connection));
	PEDANTIC(assert(drawWindow));
	PEDANTIC(assert(xDrawWindow));
	PEDANTIC(assert(drawWindow->background));
//	PEDANTIC(assert(drawWindow->flags.exposed));

	if(!(connection && drawWindow)) return;

	if(0) {
		LOG_START("connection: 0x%016" PRIxPTR, (uintptr_t)connection);
		_LOG_(", drawWindow: 0x%016" PRIxPTR, (uintptr_t)drawWindow);
		LOG_END("(0x%08x)", xDrawWindow);
	}

	xcb_rectangle_t xr;

	XcbWin2XcbScaledRectangle(rP, &xr, 0);

//	cookie = xcb_poly_rectangle_checked(connection, xDrawWindow,
	cookie = xcb_poly_fill_rectangle_checked(connection, xDrawWindow,
		drawWindow->background, 1, &xr);

	if((error = xcb_request_check(connection, cookie))) {
		LOG("poly_rectangle_failed: %d", error->error_code);
		xcb_disconnect(connection);
		exit(-1);
	}

	xcb_flush(connection);
}


int XcbWinExitWindow(WinHandle const exitWindow, WinHandle const enterWindow)
{
	if(TRACE_ENTRY_START("exitWindow: 0x%016" PRIxPTR, (uintptr_t)exitWindow)) {
		LOG_END(", enterWindow: 0x%016" PRIxPTR, (uintptr_t)enterWindow);
	}

	int unmapped = XcbWinUnmapWindow(exitWindow);

	XcbWinMapWindow(enterWindow);

	TRACE_EXIT();

	return(unmapped);
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

int XcbWinMapWindow(WinHandle const theWindow)
{
	int handled = 0;
	pxcb_window_p xw = 0;

	if((_XcbWinHandle(theWindow, &xw, 0))) {
		if(!xw->flags.map.called) {
			LOG("0x%016" PRIxPTR, (uintptr_t)theWindow);
			_XcbMapWindow(xw);

			xcb_generic_event_t* e;
			for(;!handled;) {
				e = xcb_wait_for_event(pxcb_manager.connection);
				switch(e->response_type & ~0x80) {
					case XCB_EXPOSE: {
						xcb_expose_event_t* ev = (xcb_expose_event_t*)e;
						if(ev->window == xw->window) {
							pxcb_manager.activeWindow = xw;
							pxcb_manager.drawWindow = xw;

							handled = XcbExposeEvent(ev);
						}
					}break;
				}
				free(e);
			}
		}

		return(xw->flags.exposed);
	}

	return(0);
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

WinPtr XcbWinSetActiveWindow(WinHandle const winHandle)
{
	TRACE_ENTRY("winHandle: 0x%016" PRIxPTR, (uintptr_t)winHandle);

	WinHandle const exitWindow = _XcbPalmWindow(pxcb_manager.activeWindow);
	WinHandle const enterWindow = _XcbWinHandle(winHandle, &pxcb_manager.activeWindow, 0);

	XcbWinExitWindow(exitWindow, enterWindow);

	TRACE_EXIT();

	return(exitWindow);
}

WinPtr XcbWinSetDrawWindow(WinHandle winHandle)
{
	WinPtr drawWindow = XcbWinGetDrawWindow();

	(void)_XcbWinHandle(winHandle, &pxcb_manager.drawWindow, 0);

	return(drawWindow);
}

int XcbWinUnmapWindow(WinHandle const theWindow)
{
	int handled = 0;
	pxcb_window_p xw = 0;

	if(_XcbWinHandle(theWindow, &xw, 0)) {
//		LOG("xw: 0x%016" PRIxPTR, (uintptr_t)xw);
		if(!xw->flags.unmap.called) {
//			LOG("theWindow: 0x%016" PRIxPTR, (uintptr_t)theWindow);
			_XcbUnmapWindow(xw);

			xcb_generic_event_t* e;
			for(;!handled;) {
				e = xcb_wait_for_event(pxcb_manager.connection);
				switch(e->response_type & ~0x80) {
					case XCB_UNMAP_NOTIFY: {
						xcb_unmap_notify_event_t* ev = (xcb_unmap_notify_event_t*)e;
						if(ev->window == xw->window)
							handled = XcbUnmapNotifyEvent(ev);
					}break;
				}
				free(e);
			}
		}

		return(!xw->flags.exposed);
	}

	return(1);
}
