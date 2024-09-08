#include "config.h"

#include "xForm.h"
#include "xWindow.h"

/* **** */

#include "sdk/include/Core/System/SystemMgr.h"
#include "sdk/include/Libraries/PalmOSGlue/WinGlue.h"
#include "xEvent.h"

/* **** */

#include "XcbWinMgr.h"

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

	XcbCreateWindow(windowP, bounds, windowP->frameType.bits.width);
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

void _WinSetVisible(WinHandle winHandle, Boolean visible)
{
	WinPtr theWindow = winHandle;

	theWindow->windowFlags.visible = visible;
}

/* **** */

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

	return(windowP);
}

void WinDeleteWindow(WinHandle winHandle, Boolean eraseIt)
{
	PEDANTIC(assert(winHandle));

	if(!winHandle) return;

	XcbWinDeleteWindow(winHandle, eraseIt);

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

void WinDrawWindow(WinPtr const windowP)
{
	PEDANTIC(assert(windowP));

	LOG();

	if(!windowP) return;

	pxcb_window_p xwP = XcbDrawWindow_start(windowP);

	LOG();

	XcbDrawWindow_end(xwP);
}

void WinEraseRectangle(const RectangleType* rP, UInt16 cornerDiam)
{
	PEDANTIC(assert(rP));

	XcbWinEraseRectangle(rP, cornerDiam);
}

WinHandle WinGetActiveWindow(void)
{ return(XcbWinGetActiveWindow()); }

WinHandle WinGetDrawWindow(void)
{ return(XcbWinGetDrawWindow()); }

WinPtr WinGetFirstWindow(void)
{ return(XcbWinGetFirstWindow()); }

WinPtr WinGetNextWindow(WinPtr windowP)
{
	PEDANTIC(assert(windowP));

	return(XcbWinGetNextWindow(windowP));
}

void WinGetWindowExtent(Coord *const extentX, Coord *const extentY)
{ return(_WinGetWindowExtent(WinGetDrawWindow(), extentX, extentY)); }

Boolean WinHandleEvent(EventPtr eventP)
{
	PEDANTIC(assert(eventP));

	if(!eventP) return(0);

	switch(eventP->eType) {
		case sysEventWinEnterEvent: {
			WinPtr enterWindow = eventP->data.winEnter.enterWindow;
			enterWindow->windowFlags.enabled = 1;

			window_manager.enterWindowID = 0;
			window_manager.exitedWindowID = 0;

			(void)XcbWinSetActiveWindow(enterWindow);
			(void)WinSetDrawWindow(enterWindow);

			if(enterWindow->windowFlags.dialog)
				LOG_ACTION(current_form = (FormPtr)enterWindow);
		}break;
		case sysEventWinExitEvent:
			window_manager.exitedWindowID = eventP->data.winExit.exitWindow;
			window_manager.exitWindowID = 0;
		break;
		default: break;
	}

	return(0);
}

void WinRemoveWindow(WinHandle h2window) // system use function
{
	PEDANTIC(assert(h2window));

	XcbWinRemoveWindow(h2window);
}

void WinResetClip(void)
{
	WinPtr drawWindow = XcbWinGetDrawWindow();

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
	window_manager.exitWindowID = XcbWinGetActiveWindow();
}

void WinSetClip(const RectangleType* rP)
{ return(_WinSetClip(WinGetDrawWindow(), rP)); }

WinPtr WinSetDrawWindow(WinHandle winHandle)
{
	WinPtr drawWindow = XcbWinSetDrawWindow(winHandle);
	WinPtr theWindow = winHandle;

	if(theWindow)
		theWindow->windowFlags.enabled = 1;

	return(drawWindow);
}
