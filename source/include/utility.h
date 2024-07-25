#pragma once

/* **** */

#define OFFSET_OF(_struct, _member) (&((_struct)0)->_member)
#define PGET(_dstP, _src) if(_dstP) *(_dstP) = _src
#define PSET(_srcP, _dst) if(_srcP) _dst = *(_srcP)
