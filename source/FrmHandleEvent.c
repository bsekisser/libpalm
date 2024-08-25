#include "xForm.h"
#include "xEvent.h"

/* **** */

#include "libbse/include/log.h"
#include "libbse/include/unused.h"

/* **** */

Boolean FrmHandleEvent(FormPtr formP, EventPtr eventP)
{
	switch(eventP->eType) {
		case ctlEnterEvent:
		case ctlRepeatEvent:
		case ctlSelectEvent:
//			return(CtlHandleEvent(controlP, eventP));
		case fldEnterEvent:
//			return(FldHandleEvent(fieldP, eventP));
		case fldHeightChangedEvent:
//
		case frmCloseEvent:
		case frmGadgetEnterEvent:
		case frmGadgetMiscEvent:
		case frmTitleEnterEvent:
		case frmTitleSelectEvent:
			break;
		case frmUpdateEvent:
			FrmDrawForm(formP);
			return(1);
//
		case keyDownEvent:
//
		case lstEnterEvent:
//			return(LstHandleEvent(listP, eventP));
		case menuCmdBarOpenEvent:
//
		case menuEvent:
//			return(MenuHandleEvent(menuP, eventP));
		case penDownEvent:
//
		case popSelectEvent:
//
		case sclEnterEvent:
		case sclRepeatEvent:
//			return(SclHandleEvent(barP, eventP));
		case tblEnterEvent:
//			return(TblHandleEvent(tableP, eventP));
			break;
		default:
			return(0);
	}

	LOG_ACTION(event_log_event(eventP));
	return(0);

	UNUSED(formP);
}
