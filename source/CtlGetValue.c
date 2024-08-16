#include "xControl.h"

/* **** */

#include "git/libbse/include/log.h"
//#include "git/libbse/include/unused.h"

/* **** */

Int16 CtlGetValue(const ControlType* controlP)
{
	if(!controlP) return(0);

	switch(controlP->style) {
		case feedbackSliderCtl:
		case sliderCtl:
			return(((SliderControlType*)controlP)->value);
		default:
			break;
	}

	return(controlP->attr.on);
}
