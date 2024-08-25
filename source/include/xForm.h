#pragma once

#include "config.h"

/* **** */

#define ALLOW_ACCESS_TO_INTERNALS_OF_FORMS
#define ALLOW_ACCESS_TO_INTERNALS_OF_WINDOWS

/* **** */

#include "Form.h"

/* **** */

extern FormPtr current_form;

/* **** */

FormPtr FrmGetNextForm(FormPtr formP);