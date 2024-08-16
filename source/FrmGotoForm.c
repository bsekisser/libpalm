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

	event.eType = frmCloseEvent;
	EvtAddEventToQueue(&event);

	event.eType = frmLoadEvent;
	event.data.frmLoad.formID = formID;
	EvtAddEventToQueue(&event);

	event.eType = frmOpenEvent;
	event.data.frmOpen.formID = formID;
	EvtAddEventToQueue(&event);
}
