#include "Window.h"

/* **** */

#include "libbse/include/bitops.h"
#include "libbse/include/log.h"
#include "libbse/include/unused.h"

/* **** */

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
