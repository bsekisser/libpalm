#pragma once

/* **** */

#include "config.h"

/* **** */

#define ALLOW_ACCESS_TO_INTERNALS_OF_CONTROLS

#include "Control.h"

/* **** */

#include <stdint.h>

/* **** */

typedef struct control_reserved_flags_t* control_reserved_flags_p;
typedef control_reserved_flags_p const control_reserved_flags_ref;

typedef struct control_reserved_flags_t {
	uint8_t textFree:1; // set if we own text buffer and needs to be freed
}control_reserved_flags_t;
