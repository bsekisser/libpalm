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

typedef struct FormTitleTag* FormTitlePtr;
typedef FormTitlePtr const FormTitleRef;

/* **** */

extern FormPtr current_form;

/* **** */

void FrmDeleteLabel(FormLabelRef label);
void FrmDeleteTitle(FormTitleRef title);
void FrmDrawLabel(FormLabelRef label);
void FrmDrawTitle(FormTitleRef title);
FormPtr FrmGetNextForm(FormRef formP);
