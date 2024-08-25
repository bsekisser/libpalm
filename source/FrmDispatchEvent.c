#include "xForm.h"
#include "xEvent.h"

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
//		case frmCloseEvent:
//			return(eventP->data.frmClose.formID);
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
	Boolean handled = false;

	LOG_ACTION(event_log_event(eventP));

	FormPtr formP = FrmGetFormPtr(_event_form_id(eventP)) ?: current_form;

	if(formP) {
		if(formP->handler) {
			LOG_ACTION(handled = formP->handler(eventP));
		} else
			LOG_ACTION(exit(-1));

		if(!handled)
			return(FrmHandleEvent(formP, eventP));
	}

	return(handled);
}
