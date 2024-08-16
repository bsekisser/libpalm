#include "xForm.h"

/* **** */

#include "libbse/include/log.h"
//#include "libbse/include/unused.h"

/* **** */

#include <assert.h>
#include <stdlib.h>

/* **** */

static UInt16 _event_form_id(EventPtr eventP)
{
	switch(eventP->eType) {
		case frmOpenEvent:
			return(eventP->data.frmOpen.formID);
		case frmGotoEvent:
			return(eventP->data.frmGoto.formID);
		default:
			break;
	}

	LOG_ACTION(exit(-1));
}

Boolean FrmDispatchEvent(EventPtr eventP)
{
//	event_log_event(eventP);

	WinPtr windowP = (WinPtr)current_form;
	if(!windowP) return(0);

	assert(windowP->windowFlags.dialog);

	switch(eventP->eType) {
		case nilEvent: break;
		case frmCloseEvent: break;
		case frmOpenEvent:
		case frmGotoEvent:
			return(FrmHandleEvent(FrmGetFormPtr(_event_form_id(eventP)), eventP));
		default:
			if(current_form && current_form->handler) {
				if(!current_form->handler(eventP))
					return(FrmHandleEvent(current_form, eventP));
			} else
				LOG_ACTION(exit(-1));
			break;
	}

	return(0);
}
