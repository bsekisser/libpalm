#include "config.h"

#include "Rect.h"

/* **** */

#include "ldst.h"

/* **** */

#include <assert.h>
#include <string.h>

/* **** */

void PointTypeInit(PointType *const ptP)
{
	PEDANTIC(assert(ptP));

	ptP->x = 0;
	ptP->y = 0;
}

void* PointTypeLoad(PointType *const ptP, void* p)
{
	PEDANTIC(assert(ptP));
	PEDANTIC(assert(p));

	p = lds16be(&ptP->x, p);
	return(lds16be(&ptP->y, p));
}

void RectangleTypeInit(RectangleType *const rP)
{
	PEDANTIC(assert(rP));

	PointTypeInit(&rP->topLeft);
	PointTypeInit(&rP->extent);
}

void* RectangleTypeLoad(RectangleType *const rP, void* p)
{
	PEDANTIC(assert(rP));
	PEDANTIC(assert(p));

	p = PointTypeLoad(&rP->topLeft, p);
	return(PointTypeLoad(&rP->extent, p));
}

void RctCopyRectangle(const RectangleType* srcRectP, RectangleType* dstRectP)
{
	PEDANTIC(assert(dstRectP));
	PEDANTIC(assert(srcRectP));

	memcpy(dstRectP, srcRectP, sizeof(RectangleType));
}
