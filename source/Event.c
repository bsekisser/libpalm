#include "xEvent.h"
#include "xWindow.h"

/* **** */

#include "git/libbse/include/log.h"
#include "git/libbse/include/queue.h"
#include "git/libbse/include/unused.h"

/* **** */

#include <assert.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

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

void EvtGetEvent(EventType* eventP, Int32 timeout)
{
	assert(eventP);

	if(window_manager.exitWindowID) {
		eventP->eType = winExitEvent;
		eventP->data.winExit.exitWindow = window_manager.exitWindowID;
		eventP->data.winExit.enterWindow = window_manager.enterWindowID;
	} else if(window_manager.enterWindowID) {
		eventP->eType = winEnterEvent;
		eventP->data.winEnter.exitWindow = window_manager.exitedWindowID;
		eventP->data.winEnter.enterWindow = window_manager.enterWindowID;
	} else {
		qelem_p qe = queue_dequeue_next(&event_manager.pending);
		event_qelem_p eqe = qe ? qe->data : 0;

		if(eqe) {
			memcpy(eventP, &eqe->the_event, sizeof(EventType));
			queue_enqueue(&eqe->qElem, &event_manager.free);
		} else {
			memset(eventP, 0, sizeof(EventType));
			eventP->eType = nilEvent;
		}
	}

	UNUSED(timeout);
}
