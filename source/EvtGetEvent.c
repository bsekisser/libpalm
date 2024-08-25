#include "xWindow.h"
#include "xForm.h"
#include "xEvent.h"

#include "SysEvent.h"

/* **** */

#include "libbse/include/log.h"
#include "libbse/include/queue.h"
#include "libbse/include/unused.h"

/* **** */

#include <assert.h>
#include <stdlib.h>
#include <string.h>

/* **** */

void EvtGetEvent(EventType* eventP, Int32 timeout)
{
	assert(eventP);

	qelem_p qe = queue_dequeue_next(&event_manager.pending);
	event_qelem_p eqe = qe ? qe->data : 0;

	if(0 && eqe)
		LOG_ACTION(event_log_event(&eqe->the_event));

	if(eqe) {
		memcpy(eventP, &eqe->the_event, sizeof(EventType));
		queue_enqueue(&eqe->qElem, &event_manager.free);
	} else if(window_manager.exitWindowID) {
		eventP->eType = winExitEvent;
		eventP->data.winExit.exitWindow = window_manager.exitWindowID;
		eventP->data.winExit.enterWindow = window_manager.enterWindowID;
	} else if(window_manager.enterWindowID) {
		eventP->eType = winEnterEvent;
		eventP->data.winEnter.exitWindow = window_manager.exitedWindowID;
		eventP->data.winEnter.enterWindow = window_manager.enterWindowID;
	} else {
		memset(eventP, 0, sizeof(EventType));
		eventP->eType = nilEvent;
	}

	UNUSED(timeout);
}
