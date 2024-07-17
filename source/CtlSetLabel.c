#include "Control.h"

/* **** */

#include "git/libbse/include/log.h"
//#include "git/libbse/include/unused.h"

/* **** */

void CtlSetLabel(ControlType* controlP, const Char* newLabel)
{
	if(!controlP) return;

	if(controlP->attr.graphical)
		return;

	switch(controlP->style) {
		case feedbackSliderCtl:
		case sliderCtl:
			return;
		default:
			controlP->text = (void*)newLabel;
			LOG("TODO: resize control");
			break;
	}

	if(controlP->attr.visible)
		CtlDrawControl(controlP);
}
