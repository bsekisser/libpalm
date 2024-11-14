#pragma once

#include "config.h"

/* **** */

#define ALLOW_ACCESS_TO_INTERNALS_OF_FORMS
#define ALLOW_ACCESS_TO_INTERNALS_OF_WINDOWS

#include "sdk/include/Core/UI/Form.h"

/* **** */

typedef struct FormLabelTag* FormLabelTypePtr;
typedef struct FormObjAttrTag* FormObjAttrTypePtr;
typedef struct FormObjListTag* FormObjListTypePtr;
typedef struct FormType* FormPtr;

/* **** */

extern FormPtr current_form;

/* **** */

void FrmDeleteLabel(FormLabelType *const label);
void FrmDrawLabel(FormLabelType *const label);
FormPtr FrmGetNextForm(FormPtr formP);
