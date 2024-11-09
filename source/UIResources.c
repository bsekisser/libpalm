#include "config.h"

#include "xUIResources.h"

/* **** */

#include "DataMgr.h"
#include "xForm.h"
#include "xRect.h"
#include "xMemoryMgr.h"

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

static void _ResLoadFormObject(FormObjListTypePtr const objectP,
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

	FormPtr const formP = MemPtrNewClear(sizeof(FormType));
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
		FormObjListTypePtr objectP = &formP->objects[i];

		const uint16_t objID = uint16be(&p);
		const uint32_t objType = uint32be(&p);

		const uint32_t objTypeString[2] = { htole32(be32toh(objType)), 0 };

		switch(objType) {
			case 'tLBL':
				_ResLoadFormObject(objectP, frmLabelObj, objID, ResLoadFormLabel);
				break;
			case 'Talt':
			case 'tBTN':
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
			case 'tTTL':
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

void* ResLoadFormLabel(const uint16_t rscID)
{
	MemHandle const h2label = DmGetResource('tLBL', rscID);
	PEDANTIC(assert(h2label));

	if(!h2label) LOG_ACTION(return(0));

	FormLabelType *const label = MemPtrNewClear(sizeof(FormLabelType));
	PEDANTIC(assert(label));

	MemPtr l = MemHandleLock(h2label);
	PEDANTIC(assert(l));

	/* **** */

	void* p = l;

	p = ldu16be(&label->id, p);
	p = PointTypeLoad(&label->pos, p);
	label->attr.usable = uint8(&p); p += 1;
	label->fontID = uint8(&p);

	size_t label_len = 1 + strlen(p);

	MemPtr text = MemPtrNewClear(1 + label_len);
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
