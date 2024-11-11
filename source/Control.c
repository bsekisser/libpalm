#include "config.h"

#include "xControl.h"

/* **** */

#include "git/libbse/include/log.h"
//#include "git/libbse/include/unused.h"

/* **** */

#include <assert.h>
#include <string.h>

/* **** */

void CtlDrawControl(ControlType* controlP)
{
	PEDANTIC(assert(controlP));

	LOG("TODO");

	if(!controlP) return;
	if(!controlP->attr.usable) return;

	controlP->attr.visible = true;

	switch(controlP->attr.frame) {
		case boldButtonFrame:
			RctInsetRectangle(&controlP->bounds, -1);
			WinDrawRectangle(&controlP->bounds, 0);
			RctInsetRectangle(&controlP->bounds, 1);
		__attribute__((fallthrough));
		case standardButtonFrame:
			WinDrawRectangle(&controlP->bounds, 0);
		case noButtonFrame:
			break;
		case rectangleButtonFrame:
			LOG("TODO");
			break;
	}

	if(controlP->text) {
		const int len = strlen(controlP->text);

		int ascent = 12 >> 1;
		int width = len * 12;

		WinDrawChars(controlP->text, len,
			controlP->bounds.topLeft.x + (width >> 1), controlP->bounds.topLeft.y + ascent);
	}
}

Int16 CtlGetValue(const ControlType* controlP)
{
	PEDANTIC(assert(controlP));

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
	PEDANTIC(assert(controlP));

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
	PEDANTIC(assert(controlP));

	if(!controlP) return;

	controlP->attr.usable = usable;
}

void CtlSetValue(ControlType* controlP, Int16 newValue)
{
	PEDANTIC(assert(controlP));

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
	PEDANTIC(assert(controlP));

	CtlSetUsable(controlP, true);
	CtlDrawControl(controlP);
}
