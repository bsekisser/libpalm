#include "xForm.h"

/* **** */

#include "Event.h"

/* **** */

#include "git/libbse/include/log.h"
#include "git/libbse/include/unused.h"

/* **** */

void FrmGotoForm(UInt16 formID)
{
	EventType event;

	if(current_form) {
		event.eType = frmCloseEvent;
		event.data.frmLoad.formID = current_form->formId;
		EvtAddEventToQueue(&event);
	}

	event.eType = frmLoadEvent;
	event.data.frmLoad.formID = formID;
	EvtAddEventToQueue(&event);

	event.eType = frmOpenEvent;
	event.data.frmOpen.formID = formID;
	EvtAddEventToQueue(&event);
}
