#include "Preferences.h"

/* **** */

#include "libbse/include/log.h"
#include "libbse/include/unused.h"

/* **** */

Int16 PrefGetAppPreferences(const UInt32 creator, const UInt16 id,
	void *const prefs, UInt16 *const prefsSize, const Boolean saved)
{
	*prefsSize = 0;

	LOG("TODO"); return (noPreferenceFound);

	UNUSED(creator, id, prefs, saved);
}
