#include "Rect.h"

/* **** */

#include <string.h>

/* **** */

void RctCopyRectangle(const RectangleType* srcRectP, RectangleType* dstRectP)
{ memcpy(dstRectP, srcRectP, sizeof(RectangleType)); }
