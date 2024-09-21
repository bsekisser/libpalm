#pragma once

/* **** */

#include "config.h"

/* **** */

typedef struct resource_data_t** resource_data_h;
typedef struct resource_data_t* resource_data_p;

typedef struct resource_header_t** resource_header_h;
typedef struct resource_header_t* resource_header_p;

typedef struct resource_map_t** resource_map_h;
typedef struct resource_map_t* resource_map_p;

/* **** */

#include "xMemoryMgr.h"

/* **** */

#include <stdint.h>
#include <stdlib.h>

/* **** */

typedef struct resource_data_t {
	uint32_t size;
//
	void* data;
}resource_data_t;

typedef struct resource_header_t {
	uint32_t data_offset;
	uint32_t map_offset;
	uint32_t data_length;
	uint32_t map_length;
}resource_header_t;

typedef struct resource_map_t {
	resource_header_t header;
	resource_map_h next; // 4
	void* file_ref; // 2
//
	uint16_t attributes;
	uint16_t type_list_offset;
	uint16_t name_list_offset;
	uint16_t num_types;
/*
 * resource_type_entry_t type_list[];
 * resource_reference_entry_t reference_list[]
 * resource_name_entry_t name_list[]
 */
}resource_map_t;

typedef struct resource_name_entry_t {
	uint8_t length;
//
	char* name;
}resource_name_entry_t;

typedef struct resource_reference_entry_t* resource_reference_entry_p;
typedef struct resource_reference_entry_t {
	uint16_t id;
	uint16_t name_offset;
	uint8_t attributes;
	uint32_t data_offset; // 3
//
	MemHandle h; // reserved
}resource_reference_entry_t;

typedef struct resource_type_entry_t* resource_type_entry_p;
typedef struct resource_type_entry_t {
	uint32_t type;
	uint16_t count;
	uint16_t ref_list_offset;
}resource_type_entry_t;

/* **** */

extern resource_map_h current_resource;

/* **** */

MemHandle resource_get(uint32_t type, uint16_t resID);
MemHandle resource_get1(resource_map_h map, uint32_t type, uint16_t resID);
int resource_open(const char* path);
