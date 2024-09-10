#include "config.h"

#include "Preferences.h"

/* **** */

#include "libbse/include/log.h"
#include "libbse/include/unused.h"

/* **** */

#include <assert.h>

/* **** */

Int16 PrefGetAppPreferences(const UInt32 creator, const UInt16 id,
	void *const prefs, UInt16 *const prefsSize, const Boolean saved)
{
	PEDANTIC(assert(prefs));
	PEDANTIC(assert(prefsSize));

	*prefsSize = 0;

	LOG("TODO"); return (noPreferenceFound);

	UNUSED(creator, id, prefs, saved);
}

void PrefSetAppPreferences(const UInt32 creator, const UInt16 id, const Int16 version,
	const void* prefs, const UInt16 prefsSize, const Boolean saved)
{
	PEDANTIC(assert(prefs));

	LOG("TODO"); return;

	UNUSED(creator, id, version, prefs, prefsSize, saved);
}
