#include "xWindow.h"
#include "xForm.h"

#include "SystemMgr.h"
#include "xEvent.h"

/* **** */

#include "libbse/include/log.h"
//#include "libbse/include/unused.h"

/* **** */

#include <assert.h>

/* **** */

Boolean SysHandleEvent(EventPtr eventP)
{
	static unsigned nilCount = 0;

	if(nilEvent == eventP->eType)
		nilCount++;
	else
		nilCount = 0;

	assert(16 > nilCount);

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
			nilCount++;
			assert(16 > nilCount);
			return(true);
		case sysEventPenDownEvent:
		case sysEventPenMoveEvent:
		case sysEventPenUpEvent:
			break;
		case sysEventTsmConfirmEvent:
		case sysEventTsmFepButtonEvent:
		case sysEventTsmFepModeEvent:
			break;
		case sysEventWinEnterEvent: {
			WinPtr enterWindow = eventP->data.winEnter.enterWindow;

			window_manager.activeWindow = enterWindow;
			(void)WinSetDrawWindow(enterWindow);

			if(enterWindow->windowFlags.dialog)
				current_form = (FormPtr)enterWindow;

			return(true);
		}break;
		case sysEventWinExitEvent:
			break;
		default:
			return(false);
	}

	LOG_ACTION(event_log_event(eventP));

	return(false);
}
