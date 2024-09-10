#pragma once

/* **** */

#define EXPORT __attribute__((visibility("default")))

#define LOGu(_x) LOG("%s: %u", #_x, _x)
#define LOGx32(_x) LOG("%s: 0x%08x", #_x, _x)
#define LOGzu(_x) LOG("%s: %zu", #_x, _x)

typedef struct config_t {
	union {
		unsigned raw_flags;
		struct {
			unsigned exit:1;
			unsigned init:1;
		};
	}at;
	struct {
		unsigned resource:1;
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
