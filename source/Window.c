#include "xWindow.h"

/* **** */

#include "git/libbse/include/bitops.h"
#include "git/libbse/include/log.h"
#include "git/libbse/include/unused.h"

/* **** */

static void _WinGetWindowExtent(WinPtr const windowP, Coord *const extentX, Coord *const extentY)
{
	*extentX = windowP->windowBounds.extent.x;
	*extentY = windowP->windowBounds.extent.y;
}

void _WinSetClip(WinPtr windowP, const RectangleType* rP)
{
	if(!windowP) return; // intentional, fail silently.

	windowP->clippingBounds.top = rP->topLeft.x;
	windowP->clippingBounds.left = rP->topLeft.y;
	windowP->clippingBounds.bottom = rP->topLeft.x + rP->extent.x;
	windowP->clippingBounds.right = rP->topLeft.y + rP->extent.y;
}

/* **** */

void WinAddWindow(WinHandle winHandle)
{
	winHandle->nextWindow = window_manager.firstWindow;
	window_manager.firstWindow = winHandle;
}

void WinDrawBitmap(BitmapPtr bitmapP, Coord x, Coord y)
{
	LOG("TODO"); return;

	UNUSED(bitmapP, x, y);
}


void WinDrawChars(const Char* chars, Int16 len,
	Coord x, Coord y)
{
	LOG("TODO"); return;

	UNUSED(chars, len, x, y);
}

void WinDrawLine(Coord x1, Coord y1, Coord x2, Coord y2)
{
	LOG("TODO"); return;

	UNUSED(x1, y1, x2, y2);
}

void WinDrawRectangle(const RectangleType* rP, UInt16 cornerDiam)
{
	LOG("TODO"); return;

	UNUSED(rP, cornerDiam);
}

void WinEraseRectangle(const RectangleType* rP, UInt16 cornerDiam)
{
	LOG("TODO"); return;

	UNUSED(rP, cornerDiam);
}

WinHandle WinGetActiveWindow(void)
{ return(window_manager.activeWindow); }

WinHandle WinGetDrawWindow(void)
{ return(window_manager.drawWindow); }

WinPtr WinGetFirstWindow(void)
{ return(window_manager.firstWindow); }

WinPtr WinGetNextWindow(WinPtr windowP)
{ return(windowP->nextWindow); }

void WinGetWindowExtent(Coord *const extentX, Coord *const extentY)
{ return(_WinGetWindowExtent(window_manager.drawWindow, extentX, extentY)); }

void WinResetClip(void)
{ LOG("TODO"); return; }

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

	winHandle->windowFlags.enabled = 1;

	window_manager.drawWindow = winHandle;

	return(drawWindow);
}
