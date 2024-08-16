#include "xEvent.h"

/* **** */

#include "libbse/include/log.h"
#include "libbse/include/queue.h"

/* **** */

#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

/* **** */

void EvtAddEventToQueue(const EventType *const eventP)
{
//	LOG_ACTION(event_log_event(eventP));

if(0) {
	LOG_START("free: 0x%016" PRIxPTR, (uintptr_t)event_manager.free.head);
	LOG_END(", queue: 0x%016" PRIxPTR, (uintptr_t)event_manager.pending.head);
}

	qelem_p qp = queue_dequeue_next(&event_manager.free);
	if(!qp)
		LOG_ACTION(exit(-1));

if(0)
	LOG("qp: 0x%016" PRIxPTR, (uintptr_t)qp);

	event_qelem_p dstEventP = queue_dequeue_next(&event_manager.free)->data;

	memset(dstEventP, 0, sizeof(event_qelem_t));
	qp->data = dstEventP;

	memcpy(&dstEventP->the_event, eventP, sizeof(EventType));

	queue_enqueue(qp, &event_manager.pending);
}
