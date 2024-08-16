#include "xForm.h"

/* **** */

#include "UIResources.h"

/* **** */

#include "ldst.h"

/* **** */

#include "git/libbse/include/log.h"
#include "git/libbse/include/unused.h"

/* **** */

#include <stdlib.h>

/* **** */

#define LOGx(_type, _what) LOG(" %s : " _type, #_what, _what)

/* **** */

FormType* FrmInitForm(UInt16 rscID)
{
	FormType* f = calloc(1, sizeof(FormType));
	WindowType* fw = &f->window;

	fw->windowFlags.dialog = 1;

	MemHandle h2fr = DmGetResource(formRscType, rscID);
	MemPtr frd = MemHandleLock(h2fr);

	void* p = frd;

	/* **** */

	p = lds16be(&fw->windowBounds.topLeft.x, p);
	p = lds16be(&fw->windowBounds.topLeft.y, p);
	p = lds16be(&fw->windowBounds.extent.x, p);
	p = lds16be(&fw->windowBounds.extent.y, p);
	f->attr.usable = uint8(&p); p++;
	fw->windowFlags.modal = uint8(&p); p++;
	f->attr.saveBehind = uint8(&p); p++;
	p += 2;	p += 2;
	p = ldu16be(&f->formId, p);
	p = ldu16be(&f->helpRscId, p);
	p = ldu16be(&f->menuRscId, p);
	p = ldu16be(&f->defaultButton, p);
	p += 2; p += 2;

	if(1) {
		LOGx("%u", fw->windowBounds.topLeft.x);
		LOGx("%u", fw->windowBounds.topLeft.y);
		LOGx("%u", fw->windowBounds.extent.x);
		LOGx("%u", fw->windowBounds.extent.y);
		LOGx("%u", f->formId);
		LOGx("%u", f->helpRscId);
		LOGx("%u", f->menuRscId);
		LOGx("%u", f->defaultButton);
	}

	unsigned itemCount = uint16be(&p);
	LOGx("%u", itemCount);

	for(unsigned i = 0; i < itemCount; i++)
	{
		uint16_t objID = uint16be(&p);
		uint32_t objType = uint32le(&p);

		if(1) {
			LOGx("%u", objID);
			LOGx("%u", objType);
		}
	}

	/* **** */

	MemHandleUnlock(h2fr);
}
