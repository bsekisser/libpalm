#include "Control.h"

/* **** */

#include "git/libbse/include/log.h"
//#include "git/libbse/include/unused.h"

/* **** */

void CtlSetValue(ControlType* controlP, Int16 newValue)
{
	if(!controlP) return;

	switch(controlP->style) {
		case feedbackSliderCtl:
		case sliderCtl:
			((SliderControlType*)controlP)->value = newValue;
		__attribute__((fallthrough)); // is this correct?
		default:
			controlP->attr.on = (0 != newValue);
			break;
	}

	if(controlP->attr.visible)
		CtlDrawControl(controlP);
}
