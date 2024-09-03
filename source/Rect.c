#include "Rect.h"

/* **** */

#include "ldst.h"

/* **** */

#include <string.h>

/* **** */

void PointTypeInit(PointType *const ptP)
{
	ptP->x = 0;
	ptP->y = 0;
}

void* PointTypeLoad(PointType *const ptP, void* p)
{
	p = lds16be(&ptP->x, p);
	return(lds16be(&ptP->y, p));
}

void RectangleTypeInit(RectangleType *const rP)
{
	PointTypeInit(&rP->topLeft);
	PointTypeInit(&rP->extent);
}

void* RectangleTypeLoad(RectangleType *const rP, void* p)
{
	p = PointTypeLoad(&rP->topLeft, p);
	return(PointTypeLoad(&rP->extent, p));
}

void RctCopyRectangle(const RectangleType* srcRectP, RectangleType* dstRectP)
{ memcpy(dstRectP, srcRectP, sizeof(RectangleType)); }
