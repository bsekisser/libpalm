#include "xWindow.h"
#include "sdk/include/Core/System/SystemMgr.h"
#include "sdk/include/Libraries/PalmOSGlue/WinGlue.h"

#include "xRect.h"

/* **** */

#include "git/libbse/include/bitops.h"
#include "git/libbse/include/log.h"
#include "git/libbse/include/unused.h"

/* **** */

#include <assert.h>
#include <inttypes.h>
#include <stdlib.h>

/* **** */

void _WinCreateWindow(WinPtr windowP, const RectangleType* bounds, FrameType frame,
	Boolean modal, Boolean focusable)
{
	PEDANTIC(assert(windowP));
	PEDANTIC(assert(bounds));

	if(!windowP) return;

	RectangleType *const windowBounds = &windowP->windowBounds;

	if(bounds)
		RctCopyRectangle(bounds, windowBounds);
	else
		RectangleTypeInit(windowBounds);

	_WinSetClip(windowP, windowBounds);

	WinGlueSetFrameType(WinGetWindowHandle(windowP), frame);

	windowP->windowFlags.modal = modal;
	windowP->windowFlags.focusable = focusable;

	windowP->xcb.window = 0;
}

static void _WinGetWindowExtent(WinPtr const windowP, Coord *const extentX, Coord *const extentY)
{
	PEDANTIC(assert(windowP));
	PEDANTIC(assert(extentX));
	PEDANTIC(assert(extentX));

	if(extentX)
		*extentX = windowP ? windowP->windowBounds.extent.x : 0;

	if(extentY)
		*extentY = windowP ? windowP->windowBounds.extent.y : 0;
}

void _WinSetClip(WinPtr windowP, const RectangleType* rP)
{
	PEDANTIC(assert(windowP));

	if(!windowP) return; // intentional, fail silently.

	windowP->clippingBounds.top = rP ? rP->topLeft.x : 0;
	windowP->clippingBounds.left = rP ? rP->topLeft.y : 0;
	windowP->clippingBounds.bottom = rP ? rP->topLeft.x + rP->extent.x : 0;
	windowP->clippingBounds.right = rP ? rP->topLeft.y + rP->extent.y : 0;
}

/* **** */

void WinAddWindow(WinHandle winHandle) // system use function
{
	PEDANTIC(assert(winHandle));

	if(!winHandle) return;

	winHandle->nextWindow = window_manager.firstWindow;
	window_manager.firstWindow = winHandle;
}

WinHandle WinCreateWindow(const RectangleType* bounds, FrameType frame,
	Boolean modal, Boolean focusable, UInt16* error)
{
	PEDANTIC(assert(bounds));
	PEDANTIC(assert(error));

	if(error)
		*error = errNone;

	WinPtr windowP = calloc(1, sizeof(WindowType));
	if(!windowP) {
		*error = sysErrNoFreeResource;
		return(0);
	}

	_WinCreateWindow(windowP, bounds, frame, modal, focusable);

	WinAddWindow(windowP);

	return(windowP);
}

void WinDeleteWindow(WinHandle winHandle, Boolean eraseIt)
{
	PEDANTIC(assert(winHandle));

	WinPtr windowP = winHandle;

	if(window_manager.xcb.connection) {
		if(windowP->xcb.window) {
			if(eraseIt)
				LOG("TODO: eraseIt");

			xcb_unmap_window(window_manager.xcb.connection, windowP->xcb.window);
		}
	}

	WinRemoveWindow(winHandle);
	free(winHandle);
}

void WinDrawBitmap(BitmapPtr bitmapP, Coord x, Coord y)
{
	PEDANTIC(assert(bitmapP));

	LOG("TODO"); return;

	if(!bitmapP) return;

	UNUSED(x, y);
}

void WinDrawChars(const Char* chars, Int16 len,
	Coord x, Coord y)
{
	PEDANTIC(assert(chars));

	LOG("TODO"); return;

	if(!chars) return;

	UNUSED(len, x, y);
}

void WinDrawLine(Coord x1, Coord y1, Coord x2, Coord y2)
{
	LOG("TODO"); return;

	UNUSED(x1, y1, x2, y2);
}

void WinDrawRectangle(const RectangleType* rP, UInt16 cornerDiam)
{
	PEDANTIC(assert(rP));

	LOG("TODO"); return;

	if(!rP) return;

	UNUSED(cornerDiam);
}

xcb_window_t _window_manager_window_create_xcb_window(WinPtr const windowP)
{
	PEDANTIC(assert(windowP));

	LOG();

	if(!window_manager.xcb.connection) {
		window_manager.xcb.connection = xcb_connect(NULL, NULL);

		assert(window_manager.xcb.connection);

		if(!window_manager.xcb.screen) {
			window_manager.xcb.screen = xcb_setup_roots_iterator(
				xcb_get_setup(window_manager.xcb.connection)).data;

			assert(window_manager.xcb.screen);
		}
	}

	xcb_window_t xcb_window = xcb_generate_id(window_manager.xcb.connection);
	uint32_t mask =
		XCB_CW_BACK_PIXEL
		| XCB_CW_SAVE_UNDER
		| XCB_CW_EVENT_MASK;
	uint32_t values[3] = {
			window_manager.xcb.screen->black_pixel,
			1,
			XCB_EVENT_MASK_BUTTON_PRESS	| XCB_EVENT_MASK_KEY_RELEASE,
		};

	xcb_create_window(window_manager.xcb.connection,
		XCB_COPY_FROM_PARENT,
		xcb_window,
		window_manager.xcb.screen->root,
		windowP->windowBounds.topLeft.x, windowP->windowBounds.topLeft.y,
		windowP->windowBounds.extent.x, windowP->windowBounds.extent.y,
		windowP->frameType.bits.width,
		XCB_WINDOW_CLASS_INPUT_OUTPUT,
		window_manager.xcb.screen->root_visual,
		mask, &values);

	return(xcb_window);
}

void WinDrawWindow(WinPtr const windowP)
{
	PEDANTIC(assert(windowP));

	LOG();

	if(!windowP->xcb.window) {
		windowP->xcb.window = _window_manager_window_create_xcb_window(windowP);
		assert(windowP->xcb.window);
	}

	LOG();

	xcb_map_window(window_manager.xcb.connection, windowP->xcb.window);

	xcb_flush(window_manager.xcb.connection);
}

void WinEraseRectangle(const RectangleType* rP, UInt16 cornerDiam)
{
	PEDANTIC(assert(rP));

	LOG("TODO"); return;

	if(!rP) return;

	UNUSED(rP, cornerDiam);
}

WinHandle WinGetActiveWindow(void)
{ return(window_manager.activeWindow); }

WinHandle WinGetDrawWindow(void)
{ return(window_manager.drawWindow); }

WinPtr WinGetFirstWindow(void)
{ return(window_manager.firstWindow); }

WinPtr WinGetNextWindow(WinPtr windowP)
{
	PEDANTIC(assert(windowP));

	return(windowP ? windowP->nextWindow : 0);
}

void WinGetWindowExtent(Coord *const extentX, Coord *const extentY)
{ return(_WinGetWindowExtent(window_manager.drawWindow, extentX, extentY)); }

void WinRemoveWindow(WinHandle h2window) // system use function
{
	PEDANTIC(assert(h2window));

	const WinPtr the_window = h2window;
	WinPtr lhs = window_manager.firstWindow;

	if(0) {
		LOG_START("the_window: 0x%016" PRIxPTR, (uintptr_t)the_window);
		LOG_END(", firstWindow: 0x%016" PRIxPTR, (uintptr_t)lhs);
	}

	if(the_window == lhs) {
		if(0)
			LOG("matched firstWindow: 0x%016" PRIxPTR, (uintptr_t)lhs);

		window_manager.firstWindow = the_window->nextWindow;
		return;
	}

	if(lhs) do {
		if(the_window == lhs->nextWindow) {
			if(0) {
				LOG("matched lhs->nextWindow: 0x%016" PRIxPTR,
					(uintptr_t)lhs->nextWindow);
			}

			lhs->nextWindow = the_window->nextWindow;
			return;
		}

		lhs = lhs->nextWindow;

		if(0)
			LOG("lhs: 0x%016" PRIxPTR, (uintptr_t)lhs);
	}while(lhs);
}

void WinResetClip(void)
{
	WinPtr drawWindow = window_manager.drawWindow;

	if(!drawWindow) return;

	_WinSetClip(drawWindow, &drawWindow->windowBounds);
}

Err WinScreenMode(const WinScreenModeOperation operation,
	UInt32 *const widthP, UInt32 *const heightP, UInt32 *const depthP,
	Boolean *enableColorP)
{
	switch(operation) {
		case winScreenModeGetSupportsColor:
			if(enableColorP)
				*enableColorP = true;
		break;
		case winScreenModeGetSupportedDepths:
		LOG();
			if(depthP) {
				LOG();
				*depthP = (1 | 2 | ((_BV(4) | _BV(8) | _BV(16)) >> 1));
				LOG("depth: 0x%08lx", *depthP);
		}
		break;
		case winScreenModeSet:
		break;
		default:
			LOG("operation: 0x%08x", operation);
			LOG("TODO"); return(-1);
			break;
	}

	return(errNone);

	UNUSED(operation, widthP, heightP, depthP, enableColorP);
}

void WinSetActiveWindow(WinHandle winHandle)
{
	window_manager.enterWindowID = winHandle;
	window_manager.exitWindowID = window_manager.activeWindow;
}

void WinSetClip(const RectangleType* rP)
{ return(_WinSetClip(window_manager.drawWindow, rP)); }

WinPtr WinSetDrawWindow(WinHandle winHandle)
{
	WinHandle drawWindow = window_manager.drawWindow;

	if(winHandle)
		winHandle->windowFlags.enabled = 1;

	window_manager.drawWindow = winHandle;

	return(drawWindow);
}
