#include "config.h"

#include "xWindow.h"
#include "xForm.h"

#include "SystemMgr.h"
#include "xEvent.h"

/* **** */

#include "libbse/include/log.h"
#include "libbse/include/unused.h"

/* **** */

#include <assert.h>
#include <time.h>

/* **** */

Boolean SysHandleEvent(EventPtr eventP)
{
	PEDANTIC(assert(eventP));

	switch(eventP->eType)
	{
		case sysEventAppStopEvent:
			break;
		case sysEventFrmObjectFocusLostEvent:
		case sysEventFrmObjectFocusTakeEvent:
		case sysEventFrmTitleChangedEvent:
			break;
		case sysEventKeyDownEvent:
		case sysEventKeyHoldEvent:
		case sysEventKeyUpEvent:
			break;
		case sysEventNilEvent:
			return(true);
		case sysEventPenDownEvent:
		case sysEventPenMoveEvent:
		case sysEventPenUpEvent:
			break;
		case sysEventTsmConfirmEvent:
		case sysEventTsmFepButtonEvent:
		case sysEventTsmFepModeEvent:
			break;
		case sysEventWinEnterEvent:
		case sysEventWinExitEvent:
			return(WinHandleEvent(eventP));
		default:
			return(false);
	}

	LOG_ACTION(event_log_event(eventP));

	return(false);
}

UInt16 SysTicksPerSecond(void)
{
	LOG("TODO");

	return((UInt16)CLOCKS_PER_SEC);
}

Err SysUIAppSwitch(const UInt16 cardID, const LocalID dbID, const UInt16 cmd, MemPtr const cmdPBP)
{
	LOG("TODO"); return (-1);

	UNUSED(cardID, dbID, cmd, cmdPBP);
}
