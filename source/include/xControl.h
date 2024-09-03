#pragma once

/* **** */

#include "config.h"

/* **** */

#define ALLOW_ACCESS_TO_INTERNALS_OF_CONTROLS

/* **** */

#include "Control.h"

ControlPtr _CtlNewFormButtonControl(ControlPtr const ctlP, Boolean usable, Boolean leftAnchor,
	ButtonFrameType frame, Boolean nonBoldFrame);
