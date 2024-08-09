#pragma once

/* **** */

typedef struct resource_data_t** resource_data_h;
typedef struct resource_data_t* resource_data_p;

typedef struct resource_header_t** resource_header_h;
typedef struct resource_header_t* resource_header_p;

typedef struct resource_map_t** resource_map_h;
typedef struct resource_map_t* resource_map_p;

/* **** */

#include "MemoryMgr.h"

/* **** */

#include <stdint.h>
#include <stdlib.h>

/* **** */

typedef struct resource_data_t {
	size_t size; // 4
//
	void* data;
}resource_data_t;

typedef struct resource_header_t {
	off_t data_offset; // 4
	off_t map_offset; // 4
	size_t data_length; // 4
	size_t map_length; // 4
}resource_header_t;

typedef struct resource_map_t {
	resource_header_t header_copy;
	resource_map_h next; // 4
	void* file_ref; // 2
//
	uint16_t attributes;
	off_t type_list_offset; // 2
	off_t name_list_offset; // 2
	uint16_t num_types;
/*
 * resource_type_entry_t type_list[];
 * resource_reference_entry_t reference_list[]
 * resource_name_entry_t name_list[]
 */
}resource_map_t;

typedef struct resource_name_entry_t {
	size_t length; // 1
//
	char* name;
}resource_name_entry_t;

typedef struct resource_reference_entry_t* resource_reference_entry_p;
typedef struct resource_reference_entry_t {
	uint16_t id;
	off_t name_offset; // 2
	uint8_t attributes;
	off_t data_offset; // 3
//
	MemHandle h; // reserved
}resource_reference_entry_t;

typedef struct resource_type_entry_t* resource_type_entry_p;
typedef struct resource_type_entry_t {
	uint32_t type;
	uint16_t count;
	off_t ref_list_offset; // 2?
}resource_type_entry_t;

/* **** */

MemHandle resource_get(uint32_t type, uint16_t resID);
MemHandle resource_get1(resource_map_h map, uint32_t type, uint16_t resID);
//resource_map_h resource_map(resource_header_p const rh, void *const rf);
int resource_open(const char* path);
