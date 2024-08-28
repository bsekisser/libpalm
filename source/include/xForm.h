#pragma once

#include "config.h"

/* **** */

#define ALLOW_ACCESS_TO_INTERNALS_OF_FORMS
//#define ALLOW_ACCESS_TO_INTERNALS_OF_WINDOWS

#ifndef ALLOW_ACCESS_TO_INTERNALS_OF_WINDOWS
	#include "xWindow.h"
#endif

#include "sdk/include/Core/UI/Form.h"

/* **** */

#ifndef ALLOW_ACCESS_TO_INTERNALS_OF_FORMS
//	typedef struct FormAttrType* FormAttrPtr;
	typedef struct FormAttrTag {
		unsigned saveBehind:1;
		unsigned usable:1;
	}FormAttrType;

	typedef struct FormObjListTag* FormObjListTypePtr;
	typedef struct FormObjListTag {
		FormObjListTypePtr next;
	}FormObjListType;

	typedef struct FormType* FormPtr;
	typedef struct FormType {
		WindowType window;
		uint16_t formId;
		FormAttrType attr;
		WinHandle bitsBehindForm;
		FormEventHandlerPtr handler;
		uint16_t defaultButton;
		uint16_t helpRscId;
		uint16_t menuRscId;
		uint16_t numObjects;
		FormObjListTypePtr objects;
	}FormType;
#endif

/* **** */

extern FormPtr current_form;

/* **** */

FormPtr FrmGetNextForm(FormPtr formP);
