#include "SystemMgr.h"

/* **** */

#include "libbse/include/log.h"

/* **** */

#include <xcb/xcb.h>

/* **** */

#define ERR_ESAC(_esac) { _esac, #_esac }

typedef struct palm_err_t* palm_err_p;
typedef struct palm_err_t {
	Err erc;
	const char* string;
}palm_err_t;

palm_err_t palm_err[] = {
	ERR_ESAC(sysErrRomIncompatible),
	{ 0, 0 },
};

static Boolean log_palm_err(Err erc)
{
	LOG_START("rval: 0x%08x", erc);

	palm_err_p pep = palm_err;

	while(pep->erc) {
		if(erc == pep->erc)
			break;

		pep++;
	}

	if(erc == pep->erc) {
		LOG_END(" -- %s", pep->string);
		return(true);
	} else
		LOG_END();

	return(false);
}

int main(int argc, char** argv)
{
	xcb_connection_t* xc = xcb_connect(0, 0);

	/* **** */

	const UInt16 cmd = sysAppLaunchCmdNormalLaunch;
	void *const cmdPBP;
	const UInt16 launchFlags;

	const UInt32 rval = PilotMain(cmd, cmdPBP, launchFlags);

	log_palm_err(rval);

	/* **** */

	xcb_disconnect(xc);

	return(0);
}
