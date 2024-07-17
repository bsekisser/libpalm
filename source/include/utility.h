#pragma once

/* **** */

#define MEMBER_OF(s, m) (&((s)0)->m)
#define PGET(_dstP, _src) if(_dstP) *(_dstP) = _src
#define PSET(_srcP, _dst) if(_srcP) _dst = *(_srcP)
