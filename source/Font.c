#include "config.h"

#include "xFont.h"

/* **** */

#include "libbse/include/log.h"
#include "libbse/include/unused.h"

/* **** */

#include <assert.h>
#include <string.h>

/* **** */

void FntCharsInWidth(Char const *string,
	Int16 *const stringWidthP, Int16 *const stringLengthP,
	Boolean *const fitWithinWidth)
{
	PEDANTIC(assert(string));

	unsigned len = strlen(string);

	do { // strip trailing spaces and tabs
		const Char c = string[len];
		switch(c) {
			case ' ':
			case '\t':
				len--;
			default:
				break;
		}
	}while(string[len] && len);

	LOG("TODO"); return;

	UNUSED(stringWidthP, stringLengthP, fitWithinWidth);
}

FontID FntSetFont(FontID font)
{
	LOG("TODO"); return(0);

	UNUSED(font);
}
