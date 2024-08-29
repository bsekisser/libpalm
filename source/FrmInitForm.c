#include "xForm.h"

/* **** */

#include "UIResources.h"

/* **** */

#include "ldst.h"

/* **** */

#include "git/libbse/include/log.h"
#include "git/libbse/include/unused.h"

/* **** */

#include <endian.h>
#include <stdlib.h>
#include <string.h>

/* **** */

#define LOGx(_f, _what, ...) LOG(" %s : " _f, #_what, _what, ##__VA_ARGS__)

/* **** */

FormType* FrmInitForm(UInt16 rscID)
{
	FormType f;
	WinPtr fw = &f.window;
	RectanglePtr bounds = &fw->windowBounds;

	f.handler = 0;
	f.focus = 0;
	fw->windowFlags.dialog = 1;

	MemHandle h2fr = DmGetResource(formRscType, rscID);
	MemPtr frd = MemHandleLock(h2fr);

	void* p = frd;

	/* **** */

	p = lds16be(&bounds->topLeft.x, p);
	p = lds16be(&bounds->topLeft.y, p);
	p = lds16be(&bounds->extent.x, p);
	p = lds16be(&bounds->extent.y, p);

	f.attr.usable = uint8(&p); p += 1;
	fw->windowFlags.modal = uint8(&p); p += 1;
	f.attr.saveBehind = uint8(&p); p += 1;

	p += 2;	p += 2;

	p = ldu16be(&f.formId, p);
	p = ldu16be(&f.helpRscId, p);
	p = ldu16be(&f.menuRscId, p);
	p = ldu16be(&f.defaultButton, p);

	p += 2; p += 2;

	if(1) {
		LOGx("%u", bounds->topLeft.x);
		LOGx("%u", bounds->topLeft.y);
		LOGx("%u", bounds->extent.x);
		LOGx("%u", bounds->extent.y);
		LOGx("%u", f.formId);
		LOGx("%u", f.helpRscId);
		LOGx("%u", f.menuRscId);
		LOGx("%u", f.defaultButton);
	}

	p = ldu16be(&f.numObjects, p);
	if(1)
		LOGx("%u", f.numObjects);

	const size_t objPtrAlloc = sizeof(f.objects) * f.numObjects;
	const size_t formAlloc = sizeof(FormType) + objPtrAlloc;

	FormPtr formP = calloc(1, formAlloc);
	memcpy(formP, &f, formAlloc);

	void* objectP = &formP->objects;

	for(unsigned i = 0; i < f.numObjects; i++)
	{
		uint16_t objID = uint16be(&p);
		uint32_t objType[2] = { htole32(uint32be(&p)), 0 };
//		uint32_t objType[2] = { uint32le(&p), 0 };

		switch(objType[0]) {
			case 'tFBM':
//				*objectP++ = load_form_bitmap(objID, objType[0]);
				break;
			default:
				LOGx("%u", objID);
				LOGx("0x%08x (%s)", objType[0], (char*)&objType[0]);
				LOG_ACTION(exit(-1));
				break;
		}
	}

	/* **** */

	_WinCreateWindow(&formP->window, bounds, simpleFrame, fw->windowFlags.modal, true);
	WinAddWindow(&formP->window);

	MemHandleUnlock(h2fr);
	MemHandleFree(h2fr);

	return(formP);
}
