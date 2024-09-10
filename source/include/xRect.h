#pragma once

/* **** */

#include "sdk/include/Core/System/Rect.h"

/* **** */

void __rect_log_point(const PointType* ptP, const char* name,
	const char* func, const unsigned line);
void __rect_log_rectangle(const RectangleType* rP, const char* name,
	const char* func, const unsigned line);

void PointTypeInit(PointType *const ptP);
void* PointTypeLoad(PointType *const ptP, void* p);
void RectangleTypeInit(RectangleType *const rP);
void* RectangleTypeLoad(RectangleType *const rP, void* p);

/* **** */

#define LOG_POINT(_p2pt) __rect_log_point(_p2pt, #_p2pt, __FUNCTION__, __LINE__)
#define LOG_RECTANGLE(_p2r) __rect_log_rectangle(_p2r, #_p2r, __FUNCTION__, __LINE__)
