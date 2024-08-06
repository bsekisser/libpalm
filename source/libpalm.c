#include "libpalm.h"

/* **** */

#include "SystemMgr.h"
#include "SysUtils.h"

/* **** */

#include "libbse/include/bin.h"
#include "libbse/include/log.h"
#include "libbse/include/unused.h"

/* **** */

#include <xcb/xcb.h>

/* **** */

int main(int argc, char** argv)
{
	xcb_connection_t* xc = xcb_connect(0, 0);

	/* **** */

	bin_halloc(&libpalm->rsrc);

	const UInt16 cmd = sysAppLaunchCmdNormalLaunch;
	void *const cmdPBP = 0;
	const UInt16 launchFlags = 0;

	const UInt32 rval = PilotMain(cmd, cmdPBP, launchFlags);

	char error_string[256];
	LOG("%s", SysErrString(rval, error_string, sizeof(error_string)));

	/* **** */

	xcb_disconnect(xc);

	return(0);

	UNUSED(argc, argv);
}
