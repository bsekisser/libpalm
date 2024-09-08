#include "XcbEventMgr.h"
#include "XcbWinMgr.h"

/* **** */

#include "xEvent.h"

/* **** */

#include "git/libbse/include/unused.h"

/* **** */

Boolean XcbEvtGetEvent(EventType* eventP, Int32 timeout)
{
	xcb_generic_event_t* e;
	Boolean handled = 0;

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
			case XCB_EXPOSE: {
				xcb_expose_event_t* ev = (xcb_expose_event_t*)e;

				XcbExpose(ev->window);
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

	return(handled);
	UNUSED(timeout);
}
