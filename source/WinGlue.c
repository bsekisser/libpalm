#include "config.h"

/* **** */

#define ALLOW_ACCESS_TO_INTERNALS_OF_WINDOWS

#include "xWindow.h"
#include "sdk/include/Libraries/PalmOSGlue/WinGlue.h"

/* **** */

#include <assert.h>

/* **** */

void WinGlueSetFrameType(WinHandle winH, FrameType frame)
{
	PEDANTIC(assert(winH));

	WinPtr windowP = WinGetWindowPointer(winH);

	windowP->frameType.word = 0;
	windowP->frameType.bits.width = frame & 3;
	windowP->frameType.bits.threeD = frame && 0x10;
	windowP->frameType.bits.cornerDiam = (frame >> 8) & 0x0f;
}
