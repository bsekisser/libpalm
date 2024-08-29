#include "xWindow.h"
#include "sdk/include/Libraries/PalmOSGlue/WinGlue.h"

/* **** */

void WinGlueSetFrameType(WinHandle winH, FrameType frame)
{
	WinPtr windowP = WinGetWindowPointer(winH);

	windowP->frameType.word = 0;
	windowP->frameType.bits.width = frame & 3;
	windowP->frameType.bits.threeD = frame && 0x10;
	windowP->frameType.bits.cornerDiam = (frame >> 8) & 0x0f;
}
