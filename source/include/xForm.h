#pragma once

#include "config.h"

/* **** */

//#define ALLOW_ACCESS_TO_INTERNALS_OF_FORMS
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
		unsigned visible:1;
	}FormAttrType;

	typedef struct FormObjAttrTag* FormObjAttrTypePtr;
	typedef struct FormObjAttrTag {
		unsigned usable:1;
	}FormObjAttrType;

	typedef struct FormLabelTag* FormLabelTypePtr;
	typedef struct FormLabelTag {
		FormObjAttrType attr;
		UInt8 reserved;
		Char* text;
	}FormLabelType;

	typedef struct FormObjListTag* FormObjListTypePtr;
	typedef struct FormObjListTag {
		FormObjectKind objectType;
		FormObjListTypePtr next;
	}FormObjListType;

	typedef struct FormType* FormPtr;
	typedef struct FormType {
		WindowType window;
		UInt16 formId;
		FormAttrType attr;
		WinHandle bitsBehindForm;
		FormEventHandlerPtr handler;
		UInt16 focus;
		UInt16 defaultButton;
		UInt16 helpRscId;
		UInt16 menuRscId;
		UInt16 numObjects;
		FormObjListTypePtr objects;
	}FormType;
#endif

/* **** */

extern FormPtr current_form;

/* **** */

FormPtr FrmGetNextForm(FormPtr formP);
