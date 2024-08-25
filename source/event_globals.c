#include "xEvent.h"

/* **** */

#include "libbse/include/log.h"
#include "libbse/include/queue.h"

/* **** */

#define __EVENT_QUEUE_ALLOC__ 10

event_manager_t event_manager;
event_qelem_t event_manager_queue[__EVENT_QUEUE_ALLOC__];

/* **** */

__attribute__((constructor))
static void __event_manager_globals_init(void)
{
	LOG();

	queue_init(&event_manager.free);
	queue_init(&event_manager.pending);

	for(unsigned i = 0; i < __EVENT_QUEUE_ALLOC__; i++) {
		event_qelem_p qe = &event_manager_queue[i];

		qe->qElem.data = qe;

		queue_enqueue(&qe->qElem, &event_manager.free);
	}
}
