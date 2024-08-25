#include "xEvent.h"

/* **** */

#include "libbse/include/log.h"
#include "libbse/include/queue.h"

/* **** */

#include <assert.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

/* **** */

void EvtAddEventToQueue(const EventType *const eventP)
{
//	LOG_ACTION(event_log_event(eventP));

if(0) {
	LOG_START("free.head: 0x%016" PRIxPTR, (uintptr_t)event_manager.free.head);
	LOG_END(", pending.head: 0x%016" PRIxPTR, (uintptr_t)event_manager.pending.head);
}

	qelem_p p2fqe = queue_dequeue_next(&event_manager.free);
	if(!p2fqe)
		LOG_ACTION(exit(-1));

if(0)
	LOG("free.head.qelem_p: 0x%016" PRIxPTR, (uintptr_t)p2fqe);

	event_qelem_p const p2eqe = p2fqe->data;
	EventPtr const dstEventP = &p2eqe->the_event;

	memcpy(dstEventP, eventP, sizeof(EventType));

	queue_enqueue(p2fqe, &event_manager.pending);

//	LOG_ACTION(event_log_event((EventPtr const)eventP));
//	LOG_ACTION(event_log_event(dstEventP));
}
