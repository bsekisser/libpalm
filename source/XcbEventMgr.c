#include "XcbEventMgr.h"
#include "XcbWinMgr.h"

/* **** */

#include "xEvent.h"

/* **** */

#include "git/libbse/include/log.h"
#include "git/libbse/include/unused.h"

/* **** */

#include <string.h>

/* **** */

struct config_xcb_event_t {
	struct {
		struct {
			unsigned entry:1;
			unsigned exit:1;
		}at;
	}trace;
}xcb_event_config;

/* **** */

__attribute__((constructor))
static void __xcb_event_manager_config_init(void)
{
	AT_INIT(LOG());

	(void)memset(&xcb_event_config, 0, sizeof(xcb_event_config));

//	xcb_event_config.trace.at.entry = 1;
//	xcb_event_config.trace.at.exit = 1;
}

/* **** */

Boolean XcbEvtGetEvent(EventType* eventP, Int32 timeout)
{
	if(xcb_event_config.trace.at.entry) LOG(">>");

	xcb_generic_event_t* e;
	Boolean handled = 0;

	if(!pxcb_manager.connection)
		goto exit_no_connection;

	e = xcb_poll_for_event(pxcb_manager.connection);
	if(e) {
		switch(e->response_type & ~0x80) {
			case XCB_BUTTON_PRESS: {
				xcb_button_press_event_t* ev = (xcb_button_press_event_t*)e;
				eventP->eType = penDownEvent;
				eventP->penDown = true;
				eventP->screenX = ev->event_x;
				eventP->screenY = ev->event_y;
				handled = 1;
			}break;
			case XCB_KEY_RELEASE: {
				xcb_key_release_event_t* ev = (xcb_key_release_event_t*)e;
				switch(ev->detail) {
					case 9: // ESC
						free(e);
						xcb_disconnect(pxcb_manager.connection);
						exit(0);
						break;
					default:
						eventP->eType = keyDownEvent;
						eventP->data.keyDown.chr = ev->detail;
						eventP->data.keyDown.modifiers =
							((ev->state & (1 << 0)) &&  shiftKeyMask) // shift
							|| ((ev->state & (1 << 1)) && capsLockMask) // lock
							|| ((ev->state & (1 << 2)) && commandKeyMask); // ctrl
						eventP->screenX = ev->event_x;
						eventP->screenY = ev->event_y;
						handled = 1;
						break;
				}
			}break;
		}

		free(e);
	}

exit_no_connection:
	if(xcb_event_config.trace.at.exit) LOG("<<");

	return(handled);
	UNUSED(timeout);
}
