#include "config.h"

#include "Menu.h"

#include "xEvent.h"

/* **** */

#include "libbse/include/log.h"
#include "libbse/include/unused.h"

/* **** */

#include <assert.h>

/* **** */

void MenuEraseStatus(MenuBarType *menuP)
{
	PEDANTIC(assert(menuP));

	LOG("TODO"); return;

	UNUSED(menuP);
}

Boolean MenuHandleEvent(MenuBarType* menuP, EventType* eventP, UInt16* error)
{
	PEDANTIC(assert(menuP));
	PEDANTIC(assert(eventP));
	PEDANTIC(assert(error));

	*error = -1;

	switch(eventP->eType) {
		case keyDownEvent:
		case penDownEvent:
			break;
		default:
			return(false);
	}

	LOG_ACTION(event_log_event(eventP));
	return(false);

	UNUSED(menuP, error);
}
