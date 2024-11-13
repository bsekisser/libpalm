#include "config.h"

/* **** */

#define ALLOW_ACCESS_TO_INTERNALS_OF_FORMS
#define ALLOW_ACCESS_TO_INTERNALS_OF_WINDOWS

#include "sdk/include/Core/System/SystemMgr.h"
#include "sdk/include/Libraries/PalmOSGlue/WinGlue.h"

#include "xEvent.h"
#include "xForm.h"
#include "xMemoryMgr.h"
#include "xRect.h"
#include "xWindow.h"

/* **** */

#include "XcbWinMgr.h"

/* **** */

#include "git/libbse/include/bitops.h"
#include "git/libbse/include/log.h"
#include "git/libbse/include/unused.h"

/* **** */

#include <assert.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

/* **** */

struct config_window_manager_t {
	struct {
		struct {
			unsigned entry:1;
			unsigned exit:1;
		}at;
	}trace;
}window_manager_config;

#define TRACE window_manager_config.trace

/* **** */

__attribute__((constructor))
static void window_manager_config_init(void)
{
	AT_INIT(LOG());

	memset(&window_manager_config, 0, sizeof(window_manager_config));

	window_manager_config.trace.at.entry = 1;
	window_manager_config.trace.at.exit = 1;
}

/* **** */

void _WinCreateWindow(WinPtr windowP, const RectangleType* bounds, FrameType frame,
	Boolean modal, Boolean focusable)
{
	PEDANTIC(assert(windowP));
	PEDANTIC(assert(bounds));

	if(!windowP) return;

	RectangleType *const windowBounds = &windowP->windowBounds;

	if(bounds) {
		if(bounds != windowBounds)
			RctCopyRectangle(bounds, windowBounds);
	} else
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

/* **** */

WinHandle WinCreateWindow(const RectangleType* bounds, FrameType frame,
	Boolean modal, Boolean focusable, UInt16* error)
{
	PEDANTIC(assert(bounds));
	PEDANTIC(assert(error));

	if(error)
		*error = errNone;

	WinPtr windowP = MemPtrNewClear(sizeof(WindowType));
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

	MemPtrFree(winHandle);
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
	if(TRACE_ENTRY_START()) {
		_LOG_("x: %i, y: %i", x, y);
		LOG_END(", chars: 0x%016" PRIxPTR "(%s)", (uintptr_t)chars, chars ?: "");
	}

	PEDANTIC(assert(chars));

	if(!chars) return;

	XcbWinDrawChars(chars, len, x, y);

	TRACE_EXIT();
}

void WinDrawLine(Coord x1, Coord y1, Coord x2, Coord y2)
{ XcbWinDrawLine(x1, y1, x2, y2, winPaint); }

void WinDrawRectangle(const RectangleType* rP, UInt16 cornerDiam)
{
	PEDANTIC(assert(rP));

	XcbWinDrawRectangle(rP, cornerDiam, winPaint);
}

void WinDrawWindow(WinPtr const windowP)
{
	PEDANTIC(assert(windowP));
//	PEDANTIC(assert(windowP->windowFlags.visible));

	LOG();

	if(!windowP) return;

	pxcb_window_p xwP = XcbDrawWindow_start(windowP);

	LOG();

	XcbDrawWindow_end(xwP);
}

void WinEnterWindowEvent(const struct _WinEnterEventType *const winEnter)
{
	WinHandle const enterWindow = winEnter->enterWindow;

	if(0) {
		LOG_START("enterWindow: 0x%016" PRIxPTR, (uintptr_t)winEnter->enterWindow);
		LOG_END(", exitedWindow: 0x%016" PRIxPTR, (uintptr_t)winEnter->exitWindow);
	}

	WinPtr const theWindow = winEnter->enterWindow;
	if(!theWindow) return;

	WinSetEnabled(enterWindow, true);

	window_manager.activeWindow = enterWindow;
	(void)WinSetDrawWindow(enterWindow);

	if(theWindow->windowFlags.dialog)
		LOG_ACTION(current_form = (FormPtr)enterWindow);
}

void WinEraseRectangle(const RectangleType* rP, UInt16 cornerDiam)
{
	PEDANTIC(assert(rP));

	XcbWinDrawRectangle(rP, cornerDiam, winErase);
}

void WinExitWindowEvent(const struct _WinExitEventType *const winExit)
{
	if(0) {
		LOG_START("exitWindow: 0x%016" PRIxPTR, (uintptr_t)winExit->exitWindow);
		LOG_END(", enterWindow: 0x%016" PRIxPTR, (uintptr_t)winExit->enterWindow);
	}

	WinPtr theWindow = winExit->exitWindow;

	if(theWindow) {
		theWindow->windowFlags.enabled = 0;
		theWindow->windowFlags.visible = 0;
	}

	window_manager.exitWindowID = 0;
	window_manager.exitedWindowID = winExit->exitWindow;
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

	struct _WinEnterEventType *const winEnter = &eventP->data.winEnter;
	struct _WinExitEventType *const winExit = &eventP->data.winExit;

	switch(eventP->eType) {
		case sysEventWinEnterEvent:
if(0)		LOG("sysEventWinEnterEvent: 0x%016" PRIxPTR, (uintptr_t)winEnter->enterWindow);
			if(XcbWinEnterWindow(winEnter->enterWindow, winEnter->exitWindow)) {
				WinEnterWindowEvent(winEnter);
				window_manager.enterWindowID = 0;
			}
		break;
		case sysEventWinExitEvent:
if(0)		LOG("sysEventWinExitEvent: 0x%016" PRIxPTR, (uintptr_t)winExit->exitWindow);
			if(XcbWinExitWindow(winExit->exitWindow, winEnter->enterWindow)) {
				WinExitWindowEvent(winExit);
				window_manager.exitedWindowID = winExit->exitWindow;
				window_manager.exitWindowID  = 0;
			}
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
			if(depthP) {
				LOG();
				*depthP = (1 | 2 | ((_BV(4) | _BV(8) | _BV(16)) >> 1));
if(0)			LOG("depth: 0x%08lx", *depthP);
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
	TRACE_ENTRY("winHandle: 0%016" PRIxPTR, (uintptr_t)winHandle);

	window_manager.enterWindowID = winHandle;
	window_manager.exitWindowID = XcbWinGetActiveWindow();

	TRACE_EXIT();
}

void WinSetClip(const RectangleType* rP)
{ return(_WinSetClip(WinGetDrawWindow(), rP)); }

WinPtr WinSetDrawWindow(WinHandle winHandle)
{
	WinPtr drawWindow = XcbWinSetDrawWindow(winHandle);
//	WinPtr theWindow = winHandle;

//	if(drawWindow)
//		drawWindow->windowFlags.enabled = 0;

//	if(theWindow)
//		theWindow->windowFlags.enabled = 1;

	return(drawWindow);
}

void WinSetEnabled(WinHandle const winHandle, const Boolean enabled)
{
	WinPtr const theWindow = winHandle;

	theWindow->windowFlags.enabled = enabled;
}

void WinSetVisible(WinHandle const winHandle, const Boolean visible)
{
	WinPtr const theWindow = winHandle;

	theWindow->windowFlags.visible = visible;
}
