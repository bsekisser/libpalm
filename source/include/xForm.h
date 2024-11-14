#pragma once

#include "config.h"

/* **** */

#include "sdk/include/Core/UI/Form.h"

/* **** */

typedef FormEventHandlerPtr const FormEventHandlerRef;

typedef struct FormLabelTag* FormLabelPtr;
typedef FormLabelPtr const FormLabelRef;

typedef struct FormObjAttrTag* FormObjAttrPtr;
typedef FormObjAttrPtr const FormObjAttrRef;

typedef struct FormObjListTag* FormObjListPtr;
typedef FormObjListPtr const FormObjListRef;

typedef FormObjectType* FormObjectPtr;
typedef FormObjectPtr const FormObjectRef;

typedef FormPtr const FormRef;

/* **** */

extern FormPtr current_form;

/* **** */

void FrmDeleteLabel(FormLabelRef label);
void FrmDrawLabel(FormLabelRef label);
FormPtr FrmGetNextForm(FormRef formP);
