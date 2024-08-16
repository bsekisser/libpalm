#pragma once

#include "config.h"

/* **** */

typedef struct event_manager_t** event_manager_h;
typedef struct event_manager_t* event_manager_p;

typedef struct event_qelem_t* event_qelem_p;

/* **** */

#include "Event.h"

/* **** */

#include "libbse/include/queue.h"

/* **** */

typedef struct event_manager_t {
	queue_t free;
	queue_t pending;
}event_manager_t;

typedef struct event_qelem_t {
	qelem_t qElem;
	EventType the_event;
}event_qelem_t;

/* **** */

extern event_manager_t event_manager;

/* **** */

void event_log_event(EventPtr const eventP);
