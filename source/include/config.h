#pragma once

/* **** */

#include "git/libbse/include/log.h"
#include "git/libbse/include/unused.h"

/* **** */

#define EXPORT __attribute__((visibility("default")))

typedef struct config_t {
	union {
		unsigned raw_flags;
		struct {
			unsigned exit:1;
			unsigned init:1;
		};
	}at;
	struct {
		struct {
			unsigned initForm:1;
		}form;
	}info;
	union {
		unsigned raw_flags;
		struct {
			unsigned debug:1;
			unsigned pedantic:1;
		};
	};
}config_t;

extern config_t config;

#define AT_EXIT(_x) if(config.at.exit) { _x; }
#define AT_INIT(_x) if(config.at.init) { _x; }
#define DEBUG(_x) if(config.debug) { _x; }
#define PEDANTIC(_x) if(config.pedantic) { _x; }

#define _TRACEx_y(_x, _y, _f, ...) \
	({ \
		const int _trace = TRACE.at._x; \
		\
		if(_trace) { \
			LOG##_y(_f, ##__VA_ARGS__); \
		} \
		\
		_trace; \
	})


#define TRACEx(_x, _f, ...) _TRACEx_y(_x,, _f, ##__VA_ARGS__)
#define TRACEx_START(_x, _f, ...) _TRACEx_y(_x, _START, _f, ##__VA_ARGS__)

#define TRACE_ENTRY(_f, ...) TRACEx(entry, ">> " _f, ##__VA_ARGS__)
#define TRACE_ENTRY_START(_f, ...) TRACEx_START(entry, ">> " _f, ##__VA_ARGS__)

#define TRACE_EXIT(_f, ...) TRACEx(exit, "<< " _f, ##__VA_ARGS__)
#define TRACE_EXIT_START(_f, ...) TRACEx_START(exit, "<< " _f, ##__VA_ARGS__)
