#include "SystemMgr.h"

/* **** */

#include "libbse/include/log.h"
//#include "libbse/include/unused.h"

/* **** */

#include <assert.h>

/* **** */

Boolean SysHandleEvent(EventPtr eventP)
{
	static unsigned nilCount = 0;

	if(nilEvent == eventP->eType)
		nilCount++;
	else
		nilCount = 0;

	assert(16 > nilCount);

//	event_log_event(eventP);

	LOG("TODO"); return(false);
}
