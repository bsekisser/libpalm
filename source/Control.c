#include "xControl.h"

/* **** */

#include "git/libbse/include/log.h"
//#include "git/libbse/include/unused.h"

/* **** */

void CtlDrawControl(ControlType* controlP)
{
	LOG("TODO");

	if(!controlP) return;

	controlP->attr.visible = true;
}

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

void CtlSetUsable(ControlType* controlP, Boolean usable)
{
	if(!controlP) return;

	controlP->attr.usable = usable;
}

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

void CtlShowControl(ControlType* controlP)
{
	CtlSetUsable(controlP, true);
	CtlDrawControl(controlP);
}
