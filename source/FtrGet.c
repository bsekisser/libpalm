#include "FeatureMgr.h"
#include "SystemMgr.h"

/* **** */

#include "libbse/include/log.h"
//#include "libbse/include/unused.h"

/* **** */

#include <inttypes.h>

/* **** */

Err FtrGet(UInt32 creator, UInt16 featureNum, UInt32* valueP)
{
	switch(creator) {
		case sysFtrCreator:
			switch(featureNum) {
				case sysFtrNumROMVersion:
//					*valueP = sysMakeROMVersion(2, 0, 0, 0, 0);
					*valueP = sysMakeROMVersion(5, 3, 0, 0, 0);
//					*valueP = sysMakeROMVersion(5, 3, 0, sysROMStageRelease, 0);
				return(errNone);
			}
		break;
	};

	if(1) {
		const char *cc = (void*)&creator;
		const UInt32 value = valueP ? *valueP : 0;

		LOG_START("creator: %c%c%c%c", cc[0], cc[1], cc[2], cc[3]);
		_LOG_(", featureNum: 0x%04x", featureNum);
		LOG_END(", valueP: [0x%016" PRIxPTR "](0x%08lx)", (uintptr_t)valueP, value);

		LOG("TODO"); return(-1);
	}

//	UNUSED(creator, featureNum, valueP);
}
