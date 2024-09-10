#include "config.h"

/* **** */

#include "SystemMgr.h"
#include "SysUtils.h"
#include "xResourceMgr.h"

/* **** */

#include "libbse/include/bin.h"
#include "libbse/include/log.h"
#include "libbse/include/unused.h"

/* **** */

#include <xcb/xcb.h>

/* **** */

config_t config = {
//	.at.raw_flags = ~0U,
	.debug = 1,
};

/* **** */

__attribute__((constructor))
void __main_init(void)
{
	AT_INIT(LOG());
}

int main(int argc, char** argv)
{
//	DEBUG(LOGzu(sizeof(config_t)));

//	xcb_connection_t* xc = xcb_connect(0, 0);

	/* **** */

//	if(resource_open("MerchantBW.rsrc"))
//		return(-1);

//	if(resource_open("MerchantGray.rsrc"))
//		return(-1);

	if(resource_open("Merchant.rsrc"))
		return(-1);

	if(resource_open("MerchantColor.rsrc"))
		return(-1);

	const UInt16 cmd = sysAppLaunchCmdNormalLaunch;
	void *const cmdPBP = 0;
	const UInt16 launchFlags = 0;

	const UInt32 rval = PilotMain(cmd, cmdPBP, launchFlags);

	char error_string[256];
	LOG("%s", SysErrString(rval, error_string, sizeof(error_string)));

	/* **** */

//	xcb_disconnect(xc);

	return(0);

	UNUSED(argc, argv);
}
