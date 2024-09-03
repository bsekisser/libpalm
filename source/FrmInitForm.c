#include "config.h"

#include "xControl.h"
#include "xForm.h"

/* **** */

#include "DataMgr.h"
#include "StringMgr.h"
#include "UIResources.h"
#include "xRect.h"

/* **** */

#include "ldst.h"

/* **** */

#include "git/libbse/include/err_test.h"
#include "git/libbse/include/log.h"
#include "git/libbse/include/unused.h"

/* **** */

#include <assert.h>
#include <endian.h>
#include <stdlib.h>
#include <string.h>

/* **** */

#define LOGx(_f, _what, ...) LOG(" %s : " _f, #_what, _what, ##__VA_ARGS__)

/* **** */

static void _load_form_button(FormObjListTypePtr objListItemP, UInt16 objID, UInt32 objType)
{
	LOG();

	objListItemP->objectType = frmControlObj;

	MemHandle h2r = DmGetResource(objType, objID);
	void* p = (void*)MemHandleLock(h2r);

	ControlType ctl;

	p = ldu16be(&ctl.id, p);
	p = RectangleTypeLoad(&ctl.bounds, p);
	Boolean usable = uint8(&p); p += 1;
	Boolean leftAnchor = uint8(&p); p += 1;
	UInt8 frame = uint8(&p); p += 1;
	Boolean nonBoldFrame = uint8(&p); p += 1;
	ctl.font = uint8(&p);
	ctl.text = p;

	MemHandleUnlock(p);
	DmReleaseResource(h2r);

	ControlType *const ctlP = _CtlNewFormButtonControl(&ctl, usable, leftAnchor, frame, nonBoldFrame);

	objListItemP->object.control = ctlP;
}

static void _load_form_tittle(FormObjListTypePtr objListItemP, UInt16 objID, UInt32 objType)
{
	LOG();

	objListItemP->objectType = frmTitleObj;
	objListItemP->object.title = calloc(1, sizeof(FormTitleType));
	ERR_NULL(objListItemP->object.title);

	FormTitleType *const titleP = objListItemP->object.title;

	MemHandle h2tr = DmGetResource(objType, objID);
	MemPtr p2tr = MemHandleLock(h2tr);

	void* p = p2tr;

	titleP->text = calloc(1, StrLen(p));
	ERR_NULL(titleP->text);

	p = StrCopy(titleP->text, p);

	LOG();

	if(config.info.form.initForm) {
		LOGx("%s", titleP->text);
	}

	MemHandleUnlock(h2tr);
	DmReleaseResource(h2tr);
}

FormType* FrmInitForm(UInt16 rscID)
{
	LOG();

	FormType f;
	WinPtr fw = &f.window;
	RectanglePtr bounds = &fw->windowBounds;

	f.handler = 0;
	f.focus = 0;
	fw->windowFlags.dialog = 1;

	MemHandle h2fr = DmGetResource(formRscType, rscID);
	PEDANTIC(assert(h2fr));

	if(!h2fr) return(0);

	MemPtr frd = MemHandleLock(h2fr);
	PEDANTIC(assert(frd));

	void* p = frd;

	/* **** */

	p = RectangleTypeLoad(bounds, p);

	f.attr.usable = uint8(&p); p += 1;
	fw->windowFlags.modal = uint8(&p); p += 1;
	f.attr.saveBehind = uint8(&p); p += 1;

	p += 2;	p += 2;

	p = ldu16be(&f.formId, p);
	p = ldu16be(&f.helpRscId, p);
	p = ldu16be(&f.menuRscId, p);
	p = ldu16be(&f.defaultButton, p);

	p += 2; p += 2;

	if(config.info.form.initForm) {
		LOG_RECTANGLE(bounds);
		LOGu(f.formId);
		LOGu(f.helpRscId);
		LOGu(f.menuRscId);
		LOGu(f.defaultButton);
	}

	p = ldu16be(&f.numObjects, p);
	if(config.info.form.initForm)
		LOGu(f.numObjects);

	const size_t objPtrAlloc = sizeof(f.objects) * f.numObjects;
	const size_t formAlloc = sizeof(FormType) + objPtrAlloc;

	FormPtr formP = calloc(1, formAlloc);
	ERR_NULL(formP);

	memcpy(formP, &f, formAlloc);

	LOG();

	FormObjListTypePtr objectsP = calloc(f.numObjects, sizeof(FormObjListType));
	ERR_NULL(objectsP);

	formP->objects = objectsP;

	for(unsigned i = 0; i < f.numObjects; i++)
	{
		FormObjListTypePtr objectP = &objectsP[i];

		uint16_t objID = uint16be(&p);
		uint32_t objType = uint32be(&p);

		uint32_t objTypeString[2] = { htole32(be32toh(objType)), 0 };

		switch(objType) {
			case 'Talt':
//				load_form_alert(objectP, objectP, objID, objType);
				break;
			case 'tBTN':
				_load_form_button(objectP, objID, objType);
				break;
			case 'tCBX':
//				load_form_checkbox(objectP, objID, objType);
				break;
			case 'tFBM':
//				load_form_bitmap(objectP, objID, objType);
				break;
			case 'tFLD':
//				load_form_field(objectP, objID, objType);
				break;
			case 'tGDT':
//				load_form_gadget(objectP, objID, objType);
				break;
			case 'tGSI':
//				load_form_graffiti_shift_indicator(objectP, objID, objType);
				break;
			case 'tLBL':
//				load_form_label(objectP, objID, objType);
				break;
			case 'tLST':
//				load_form_list(objectP, objID, objType);
				break;
			case 'tPBN':
//				load_form_push_button(objectP, objID, objType);
				break;
			case 'tPUT':
//				load_form_popup_trigger(objectP, objID, objType);
				break;
			case 'tREP':
//				load_form_repeating_button(objectP, objID, objType);
				break;
			case 'tSCL':
//				load_form_scrollbar(objectP, objID, objType);
				break;
			case 'tSLT':
//				load_form_selector_trigger(objectP, objID, objType);
				break;
			case 'tTBL':
//				load_form_table(objectP, objID, objType);
				break;
			case 'tTTL':
				_load_form_tittle(objectP, objID, objType);
				break;
			default:
				LOGu(objID);
				LOGx("0x%08x (%s)", objType, (char*)&objTypeString);
				LOG_ACTION(exit(-1));
				break;
		}
	}

	LOG();

	/* **** */

	_WinCreateWindow(&formP->window, bounds, simpleFrame, fw->windowFlags.modal, true);
//	WinAddWindow(&formP->window);

	formP->attr.usable = 1;

	MemHandleUnlock(h2fr);
	DmReleaseResource(h2fr);

	return(formP);
}
