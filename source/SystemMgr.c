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

#if 0
	static unsigned nilCount = 0;

	if(nilEvent == eventP->eType)
		nilCount++;
	else
		nilCount = 0;

	assert(16 > nilCount);
#endif

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
#if 0
			nilCount++;
			assert(16 > nilCount);
#endif
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
			window_manager.enterWindowID = 0;
			window_manager.exitedWindowID = 0;

			window_manager.activeWindow = enterWindow;
			(void)WinSetDrawWindow(enterWindow);

			if(enterWindow->windowFlags.dialog)
				LOG_ACTION(current_form = (FormPtr)enterWindow);

			return(true);
		}break;
		case sysEventWinExitEvent:
			window_manager.exitedWindowID = eventP->data.winExit.exitWindow;
			window_manager.exitWindowID = 0;
			break;
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
