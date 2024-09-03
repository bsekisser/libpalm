#include "config.h"

#include "xControl.h"

/* **** */

#include "git/libbse/include/err_test.h"
#include "git/libbse/include/log.h"
//#include "git/libbse/include/unused.h"

/* **** */

#include <assert.h>
#include <stdlib.h>
#include <string.h>

/* **** */

ControlPtr _CtlNewFormButtonControl(ControlPtr const srcCtlP, Boolean usable, Boolean leftAnchor,
	ButtonFrameType frame, Boolean nonBoldFrame)
{
	PEDANTIC(assert(srcCtlP));

	ControlPtr ctlP = calloc(1, sizeof(ControlType));
	ERR_NULL(ctlP);

	memcpy(ctlP, srcCtlP, sizeof(ControlType));

	ctlP->attr.usable = usable;
	ctlP->attr.leftAnchor = leftAnchor;
	ctlP->attr.frame = frame ? (nonBoldFrame ? standardButtonFrame : boldButtonFrame) : noButtonFrame;

	return(ctlP);
}

/* **** */

void CtlDrawControl(ControlType* controlP)
{
	PEDANTIC(assert(controlP));

	LOG("TODO");

	if(!controlP) return;

	controlP->attr.visible = true;
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
