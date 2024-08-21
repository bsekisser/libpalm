#include "Event.h"

/* **** */

#include "libbse/include/log.h"

/* **** */

#include <stdlib.h>

/* **** */

#define ESAC(_esac) case _esac: LOG(#_esac); break;

#define ESAC_LIST \
	ESAC(nilEvent) \
	ESAC(penDownEvent) \
	ESAC(penUpEvent) \
	ESAC(penMoveEvent) \
	ESAC(keyDownEvent) \
	ESAC(winEnterEvent) \
	ESAC(winExitEvent) \
	ESAC(ctlEnterEvent) \
	ESAC(ctlExitEvent) \
	ESAC(ctlSelectEvent) \
	ESAC(ctlRepeatEvent) \
	ESAC(lstEnterEvent) \
	ESAC(lstSelectEvent) \
	ESAC(lstExitEvent) \
	ESAC(popSelectEvent) \
	ESAC(fldEnterEvent) \
	ESAC(fldHeightChangedEvent) \
	ESAC(fldChangedEvent) \
	ESAC(tblEnterEvent) \
	ESAC(tblSelectEvent) \
	ESAC(daySelectEvent) \
	ESAC(menuEvent) \
	ESAC(appStopEvent) \
	ESAC(frmLoadEvent) \
	ESAC(frmOpenEvent) \
	ESAC(frmGotoEvent) \
	ESAC(frmUpdateEvent) \
	ESAC(frmSaveEvent) \
	ESAC(frmCloseEvent) \
	ESAC(frmTitleEnterEvent) \
	ESAC(frmTitleSelectEvent) \
	ESAC(tblExitEvent) \
	ESAC(sclEnterEvent) \
	ESAC(sclExitEvent) \
	ESAC(sclRepeatEvent) \
	ESAC(tsmConfirmEvent) \
	ESAC(tsmFepButtonEvent) \
	ESAC(tsmFepModeEvent) \
	ESAC(attnIndicatorEnterEvent) \
	ESAC(attnIndicatorSelectEvent) \
	ESAC(menuCmdBarOpenEvent) \
	ESAC(menuOpenEvent) \
	ESAC(menuCloseEvent) \
	ESAC(frmGadgetEnterEvent) \
	ESAC(frmGadgetMiscEvent) \
	ESAC(firstINetLibEvent) \
	ESAC(firstWebLibEvent) \
	ESAC(telAsyncReplyEvent) \
	ESAC(keyUpEvent) \
	ESAC(keyHoldEvent) \
	ESAC(frmObjectFocusTakeEvent) \
	ESAC(frmObjectFocusLostEvent)
//	ESAC(winDisplayChangedEvent)

void event_log_event(EventPtr const eventP)
{
	int eType = eventP->eType;

	switch((int)eventP->eType) {
		ESAC(winDisplayChangedEvent)
//
		ESAC_LIST
		default:
			switch(eventP->eType) {
				default:
					LOG_ACTION(exit(-1));
					break;
				case firstLicenseeEvent...lastLicenseeEvent:
					LOG("liscenseeEvent: 0x%04x", eType);
					break;
				case firstUserEvent...lastUserEvent:
					LOG("userEvent: 0x%04x", eType);
					break;
			}
		break;
	}
}
