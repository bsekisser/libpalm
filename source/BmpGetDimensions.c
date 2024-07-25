#include "Bitmap.h"

/* **** */

#include "libbse/include/log.h"
#include "libbse/include/unused.h"

/* **** */

void BmpGetDimensions(const BitmapType* bitmapP,
	Coord *const widthP, Coord *const heightP, UInt16* rowBytesP)
{
	if(bitmapP)
		return;

	if(widthP)
		*widthP = bitmapP->width;

	if(heightP)
		*heightP = bitmapP->height;

	if(rowBytesP)
		*rowBytesP = bitmapP->rowBytes;
}
