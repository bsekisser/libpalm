#include "config.h"

#include "Rect.h"

/* **** */

#include "ldst.h"

/* **** */

#include "git/libbse/include/log.h"

/* **** */

#include <assert.h>
#include <string.h>

/* **** */

void __rect_log_point(const PointType* ptP, const char* name,
	const char* func, const unsigned line)
{
	PEDANTIC(assert(ptP));

	printf("%s:%04u: ", func, line);

	LOG_END("%s->x: %u, %s->y: %u",
		name, ptP->x,
		name, ptP->y);
}

void __rect_log_rectangle(const RectangleType* rP, const char* name,
	const char* func, const unsigned line)
{
	PEDANTIC(assert(rP));

	printf("%s:%04u: ", func, line);

	_LOG_("%s->topLeft.x: %u, %s->topLeft.y: %u",
		name, rP->topLeft.x,
		name, rP->topLeft.y);
	LOG_END(", %s->extent.x: %u, %s->extent.y: %u",
		name, rP->extent.x,
		name, rP->extent.y);
}

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
