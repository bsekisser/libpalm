#include "SystemMgr.h"
#include "SysUtils.h"

/* **** */

#include <stdio.h>

/* **** */

#define ERR_ESAC(_esac) { _esac, #_esac }

typedef struct palm_err_t* palm_err_p;
typedef struct palm_err_t {
	Err erc;
	char* string;
}palm_err_t;

palm_err_t palm_err[] = {
	ERR_ESAC(memErrInvalidParam),
	ERR_ESAC(sysErrRomIncompatible),
	{ 0, 0 },
};

static Char* palm_err_string(Err erc)
{
	palm_err_p pep = palm_err;

	while(pep->erc) {
		if(erc == pep->erc)
			return(pep->string);

		pep++;
	}

	return(0);
}

Char* SysErrString(const Err err, Char *const strP, const UInt16 maxLen)
{
	// TODO, for now...  we fudge it.

//	const Char** tstl0 = tstl[err >> 8];
//	const Char* p2tstl = tstl0 ? tstl[err & 0xff] : 0;
	const Char* p2tstl = palm_err_string(err);

	if(strP) {
		const char* string = p2tstl ?: "unknown error";

		snprintf(strP, maxLen, "%s (0x%04x)", string, err);
	}

	return(strP);
}
