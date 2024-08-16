#pragma once

/* **** */

#include "libbse/include/log.h"

/* **** */

#include <endian.h>
#include <stdint.h>

/* **** */

static inline
void* ldu8(uint8_t* data, uint8_t* p)
{
	*data = *p++;
	return(p);
}

static inline
void* lds16be(int16_t* data, uint16_t* p)
{
	*data = (int16_t)be16toh(*p++);
	return(p);
}

static inline
void* ldu16be(uint16_t* data, uint16_t* p)
{
	*data = be16toh(*p++);
	return(p);
}

static inline
void* ldu16le(uint16_t* data, uint16_t* p)
{
	*data = le16toh(*p++);
	return(p);
}

static inline
void* ldu32be(uint32_t* data, uint32_t* p)
{
	*data = be32toh(*p++);
	return(p);
}

static inline
void* ldu32le(uint32_t* data, uint32_t* p)
{
	*data = le32toh(*p++);
	return(p);
}

/* **** */

static inline
uint8_t uint8(void* *const p)
{
	uint8_t data = 0;

	*p = ldu8(&data, *p);

	return(data);
}


static inline
uint16_t uint16be(void* *const p)
{
	uint16_t data = 0;

	*p = ldu16be(&data, *p);

	return(data);
}

static inline
uint32_t uint24be(void* *const p)
{
	uint32_t data = 0;

	for(unsigned i = 0; i < 3; i++) {
		data <<= 8;
		data |= uint8(p);
	}

	return(data);
}

static inline
uint32_t uint32be(void* *const p)
{
	uint32_t data = 0;

	*p = ldu32be(&data, *p);

	return(data);
}

static inline
uint32_t uint32le(void* *const p)
{
	uint32_t data = 0;

	*p = ldu32le(&data, *p);

	return(data);
}
