#include "config.h"

/* **** */

#define ALLOW_ACCESS_TO_INTERNALS_OF_FORMS
#define ALLOW_ACCESS_TO_INTERNALS_OF_WINDOWS

#include "DataMgr.h"
#include "xControl.h"
#include "xForm.h"
#include "xMemoryMgr.h"
#include "xRect.h"
#include "xUIResources.h"

/* **** */

#include "ldst.h" // TODO -->> #include "libbse/include/ldst.h"
#include "libbse/include/err_test.h"
#include "libbse/include/log.h"
#include "libbse/include/unused.h"

/* **** */

#include <assert.h>
#include <stdint.h>
#include <string.h>

#define LOGx(_f, _what, ...) LOG(" %s : " _f, #_what, _what, ##__VA_ARGS__)

/* **** */

typedef void* (*FormResLoadFn)(const uint16_t rscID);

static void _ResLoadFormObject(FormObjListRef objectP,
	const FormObjectKind objectType, const uint16_t rscID, FormResLoadFn fn)
{
	objectP->objectType = objectType;
	objectP->object.ptr = fn(rscID);
}

void* ResLoadForm(const uint16_t rscID)
{
	MemHandle const h2fr = DmGetResource(formRscType, rscID);
	PEDANTIC(assert(h2fr));

	if(!h2fr) return(0);

	FormRef formP = MemPtrNewClear(sizeof(FormType));
	ERR_NULL(formP);

	FormAttrType *const attr = &formP->attr;
	WinPtr const window = &formP->window;

	MemPtr const frd = MemHandleLock(h2fr);
	PEDANTIC(assert(frd));

	void* p = frd;

	/* **** */

	p = RectangleTypeLoad(&window->windowBounds, p);

	attr->usable = uint8(&p); p += 1;
	window->windowFlags.modal = uint8(&p); p += 1;
	attr->saveBehind = uint8(&p); p += 1;

	p += 2;	p += 2;

	p = ldu16be(&formP->formId, p);
	p = ldu16be(&formP->helpRscId, p);
	p = ldu16be(&formP->menuRscId, p);
	p = ldu16be(&formP->defaultButton, p);

	p += 2; p += 2;

	p = ldu16be(&formP->numObjects, p);

	formP->objects = MemPtrNewClear(formP->numObjects * sizeof(FormObjListType));
	ERR_NULL(formP->objects);

	for(unsigned i = 0; i < formP->numObjects; i++)
	{
		FormObjListRef objectP = &formP->objects[i];

		const uint16_t objID = uint16be(&p);
		const uint32_t objType = uint32be(&p);

		const uint32_t objTypeString[2] = { htole32(be32toh(objType)), 0 };

		switch(objType) {
			case 'tBTN':
				_ResLoadFormObject(objectP, frmControlObj, objID, ResLoadFormButton);
				break;
			case 'tLBL':
				_ResLoadFormObject(objectP, frmLabelObj, objID, ResLoadFormLabel);
				break;
			case 'tTTL':
				_ResLoadFormObject(objectP, frmTitleObj, objID, ResLoadFormTitle);
				break;
			case 'Talt':
			case 'tCBX':
			case 'tFBM':
			case 'tFLD':
			case 'tGDT':
			case 'tGSI':
			case 'tLST':
			case 'tPBN':
			case 'tPUT':
			case 'tREP':
			case 'tSCL':
			case 'tSLT':
			case 'tTBL':
			default:
				LOGu(objID);
				LOGx("0x%08x (%s)", objType, (char*)&objTypeString);
//				LOG_ACTION(exit(-1));
				break;
		}
	}

	/* **** */

	MemHandleUnlock(h2fr);
	DmReleaseResource(h2fr);

	return(formP);
}

void* ResLoadFormButton(const uint16_t rscID)
{
	MemHandle const h2button = DmGetResource('tBTN', rscID);
	PEDANTIC(assert(h2button));

	if(!h2button) LOG_ACTION(return(0));

	ControlPtr const btn = MemPtrNewClear(sizeof(ControlType));
	PEDANTIC(assert(btn));

	MemPtr const b = MemHandleLock(h2button);
	PEDANTIC(assert(b));

	void* p = b;

	/* **** */

	p = ldu16be(&btn->id, p);
	p = RectangleTypeLoad(&btn->bounds, p);
	btn->attr.usable = uint8(&p); p++;
	btn->attr.leftAnchor = uint8(&p); p++;
	const int frame = uint8(&p); p++;
	const int nonBoldFrame = uint8(&p); p++;
	btn->attr.frame = frame ? (nonBoldFrame ? standardButtonFrame : boldButtonFrame) : noButtonFrame;
	btn->font = uint8(&p);
	const int len = strlen(p);
	btn->text = MemPtrNewClear(len);
	strncpy(btn->text, p, len);

	btn->style = buttonCtl;
	((control_reserved_flags_ref)&btn->reserved)->textFree = 1;

	/* **** */

	MemHandleUnlock(h2button);
	DmReleaseResource(h2button);

	return(btn);
}

void* ResLoadFormLabel(const uint16_t rscID)
{
	MemHandle const h2label = DmGetResource('tLBL', rscID);
	PEDANTIC(assert(h2label));

	if(!h2label) LOG_ACTION(return(0));

	FormLabelRef label = MemPtrNewClear(sizeof(FormLabelType));
	PEDANTIC(assert(label));

	MemPtr const l = MemHandleLock(h2label);
	PEDANTIC(assert(l));

	/* **** */

	void* p = l;

	p = ldu16be(&label->id, p);
	p = PointTypeLoad(&label->pos, p);
	label->attr.usable = uint8(&p); p += 1;
	label->fontID = uint8(&p);

	size_t label_len = 1 + strlen(p);

	MemPtr const text = MemPtrNewClear(1 + label_len);
	PEDANTIC(assert(text));

	label->text = text;
	strncpy(text, p, label_len);

	if(1) {
		LOG_START("id: 0x%04x", label->id);
		_LOG_(", pos(x: 0x%04x, y: 0x%04x)", label->pos.x, label->pos.y);
		_LOG_(", usable: %01u", label->attr.usable);
		LOG_END(", text: %s", label->text);
	};

	/* **** */

	MemHandleUnlock(h2label);
	DmReleaseResource(h2label);

	return(label);
}

void* ResLoadFormTitle(const uint16_t rscID)
{
	MemHandle const h2title = DmGetResource('tTTL', rscID);
	PEDANTIC(assert(h2title));

	if(!h2title) LOG_ACTION(return(0));

	FormTitleRef title = MemPtrNewClear(sizeof(FormTitleType));
	PEDANTIC(assert(title));

	MemPtr const t = MemHandleLock(h2title);
	PEDANTIC(assert(t));

	/* **** */

	size_t len = 1 + strlen(t);

	title->text = MemPtrNewClear(1 + len);
	PEDANTIC(assert(title->text));

	strncpy(title->text, t, len);

	if(1) {
		LOG_START("id: 0x%04x", rscID);
		LOG_END(", text: %s", title->text);
	};

	/* **** */

	MemHandleUnlock(h2title);
	DmReleaseResource(h2title);

	return(title);
}
