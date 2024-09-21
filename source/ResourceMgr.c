#include "config.h"

#include "xMemoryMgr.h"
#include "xResourceMgr.h"

#include "ldst.h"

/* **** */

#include "libbse/include/err_test.h"
#include "libbse/include/log.h"

/* **** */

#include <assert.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdint.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

/* **** */

struct config_resource_t {
	unsigned info:1;
}resource_config;

resource_map_h current_resource;

/* **** */

__attribute__((constructor))
static void __resource_manager_config_init(void)
{
	AT_INIT(LOG());

	(void)memset(&resource_config, 0, sizeof(resource_config));
//	resource_config.info = 1;
}

__attribute__((constructor))
static void __resource_manager_globals_init(void)
{
	AT_INIT(LOG());

	current_resource = 0;
}

/* **** */

static resource_data_p _resource_data(resource_reference_entry_p const rre, resource_map_p const map)
{
	PEDANTIC(assert(rre));
	PEDANTIC(assert(map));

	static resource_data_t rdt;

	const uint32_t offset = map->header.data_offset
		+ rre->data_offset;

	void* p = map->file_ref + offset;

	p = ldu32be(&rdt.size, p);
	rdt.data = p;

	if(resource_config.info) {
		LOG_START("data_offset: 0x%04x", rre->data_offset);
		_LOG_(", file_offset: 0x%08x", offset);
		LOG_END(", size: 0x%04x", rdt.size);
	}

	return(&rdt);
}

static resource_header_p _resource_header(resource_header_p const rh, void *const rf, const off_t offset)
{
	PEDANTIC(assert(rh));
	PEDANTIC(assert(rf));

	void* p = rf + offset;

	p = ldu32be(&rh->data_offset, p);
	p = ldu32be(&rh->map_offset, p);
	p = ldu32be(&rh->data_length, p);
	p = ldu32be(&rh->map_length, p);

	if(resource_config.info) {
		LOG_START("data_offset: 0x%08x", rh->data_offset);
		_LOG_(", map_offset: 0x%08x", rh->map_offset);
		_LOG_(", data_length: 0x%08x", rh->data_length);
		LOG_END(", map_length: 0x%08x", rh->map_length);
	}

	return(rh);
}

static resource_map_h _resource_map_next(resource_map_h const h2lhs, resource_map_h const h2rhs)
{
	resource_map_p const lhs = h2lhs ? *h2lhs : 0;
	resource_map_p const rhs = h2rhs ? *h2rhs : 0;
	resource_map_h const next = rhs ? rhs->next : (lhs ? 0 : current_resource);

	if(h2lhs)
		*h2lhs = rhs;

	if(h2rhs)
		*h2rhs = next ? *next : 0;

	return(next);
}

static resource_reference_entry_p _resource_reference_entry_for_id(const uint16_t resID,
	resource_type_entry_p const rte, resource_map_p const map)
{
	PEDANTIC(assert(rte));
	PEDANTIC(assert(map));

	static resource_reference_entry_t rre;

	const uint32_t offset = map->header.map_offset
		+ map->type_list_offset
		+ rte->ref_list_offset;

	void* p = map->file_ref + offset;

	if(resource_config.info) {
		LOG_START("id: %u", resID);
		LOG_END(", offset: 0x%08x, p: 0x%016" PRIxPTR, offset, (uintptr_t)p);
	}

	for(unsigned i = 0; i <= rte->count; i++) {
		p = ldu16be(&rre.id, p);
		p = ldu16be(&rre.name_offset, p);
//		p = ldu16be(&rre.attributes, p);
		p = ldu8(&rre.attributes, p);
		rre.data_offset = uint24be(&p);
		p += 4; // reserved for handle to resource

		if(resource_config.info) {
			LOG_START("id: %u", rre.id);
			_LOG_(", name_offset: 0x%04x", rre.name_offset);
			_LOG_(", attributes: 0x%02x", rre.attributes);
			LOG_END(", data_offset: 0x%06x", rre.data_offset);
		}

		if(resID == rre.id)
			return(&rre);
	}

	return(0);
}

static resource_type_entry_p _resource_type_entry_for_type(const uint32_t type, resource_map_p const map)
{
	PEDANTIC(assert(map));

	static resource_type_entry_t rte;

	const uint32_t offset = 2 + map->header.map_offset
//	const uint32_t offset = map->header.map_offset
		+ map->type_list_offset;

	void* p = map->file_ref + offset;

	const uint32_t type_string[2] = { htole32(be32toh(type)), 0 };

	if(resource_config.info) {
		LOG_START("type: 0x%08x (%s)", type, (const char*)&type_string);
		LOG_END(", offset: 0x%08x, p: 0x%016" PRIxPTR, offset, (uintptr_t)p);
	}

	for(unsigned i = 0; i <= map->num_types; i++) {
		p = ldu32be(&rte.type, p);
		p = ldu16be(&rte.count, p);
		p = ldu16be(&rte.ref_list_offset, p);

		const uint32_t type_string[2] = { htole32(be32toh(rte.type)), 0 };

		if(resource_config.info) {
			LOG_START("type: 0x%08x (%s)", rte.type, (const char*)&type_string);
			_LOG_(", count: 0x%04x", rte.count);
			LOG_END(", ref_list_offset: 0x%04x", rte.ref_list_offset);
		}

		if(type == rte.type)
			return(&rte);
	}

	switch(type) {
		case 'Tbmp':
			return(_resource_type_entry_for_type('tbmf', map));
	}

	return(0);
}

/* **** */

MemHandle resource_get(const uint32_t type, const uint16_t resID)
{
	resource_map_p lhs = 0, rhs = 0;
	resource_map_h h2map = 0;

	while((h2map = _resource_map_next(&lhs, &rhs))) {
		MemHandle const h = resource_get1(h2map, type, resID);
		if(h)
			return(h);
	};

	return(0);
}

MemHandle resource_get1(resource_map_h const h2map, const uint32_t type, const uint16_t resID)
{
	PEDANTIC(assert(h2map));

	if(!h2map) return(0);

	MemHandle rmh = 0;
	resource_map_p const map = MemHandleLock((MemHandle)h2map);
	PEDANTIC(assert(map));

	if(!map) return(0);

	// find type list element
	resource_type_entry_p const rte = _resource_type_entry_for_type(type, map);
	if(!rte) goto failExit;

	// get reference entry
	resource_reference_entry_p const rre = _resource_reference_entry_for_id(resID, rte, map);
	if(!rte) goto failExit;

	// get resource data
	resource_data_p const rd = _resource_data(rre, map);
	if(!rd) goto failExit;

	rmh = MemHandleNew(rd->size);
	PEDANTIC(assert(rmh));

	MemPtr const mhp = MemHandleLock(rmh);
	PEDANTIC(assert(mhp));

	(void)mem_handle_resource(rmh, true, true);

	if(!rmh) LOG_ACTION(goto failExit);

	if(mhp)
		memcpy(mhp, rd->data, rd->size);

	MemHandleUnlock(rmh);

failExit:
	MemHandleUnlock((MemHandle)h2map);

	return(rmh);
}

static resource_header_p resource_header(void *const rf)
{
	static resource_header_t header;

	if(!_resource_header(&header, rf, 0))
		return(0);

	resource_header_t copy;

	if(!_resource_header(&copy, rf, header.map_offset))
		return(0);

	const unsigned invalid = (header.data_length != copy.data_length)
		|| (header.data_offset != copy.data_offset)
		|| (header.map_length != copy.map_length)
		|| (header.map_offset != copy.map_offset);

	return(invalid ? 0 : &header);
}

static MemHandle resource_map(resource_header_p const rh, void *const rf)
{
	PEDANTIC(assert(rh));
	PEDANTIC(assert(rf));

	if(!(rh && rf)) return(0);

	MemHandle const h2map = MemHandleNew(sizeof(resource_map_t));
	PEDANTIC(assert(h2map));

	if(!h2map) return(0);

	resource_map_p const map = MemHandleLock(h2map);
	PEDANTIC(assert(map));
	if(!map) return(0);

	memcpy(&map->header, rh, sizeof(resource_header_t));

	void* p = 16 + rf + rh->map_offset;

	map->next = 0;
	p += 4;
	map->file_ref = rf;
	p += 2;

	/* **** */

	p = ldu16be(&map->attributes, p);
	p = ldu16be(&map->type_list_offset, p);
	p = ldu16be(&map->name_list_offset, p);
	p = ldu16be(&map->num_types, p);

	if(resource_config.info) {
		LOG_START("attrigbutes: 0x%04x", map->attributes);
		_LOG_(", type_list_offset: 0x%04x", map->type_list_offset);
		_LOG_(", name_list_offset: 0x%04x", map->name_list_offset);
		LOG_END(", num_types: %u", map->num_types);
	}

	/* **** */

	map->next = current_resource;
	current_resource = (resource_map_h)h2map;

	MemHandleUnlock(h2map);

	return(h2map);
}

int resource_open(const char* path)
{
	PEDANTIC(assert(path));

	if(!path) return(-1);

	int fd = open(path, O_RDONLY);
	if(-1 == fd) {
		perror("open");
		return(-1);
	}

	struct stat sb;
	if(-1 == fstat(fd, &sb)) {
		perror("fstat");
		return(-1);
	}

	void *const data = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	ERR_NULL(data);

//	const size_t size = sb.st_size;

	if(-1 == close(fd)) {
		perror("close");
		return(-1);
	}

	resource_header_p header = resource_header(data);
	if(!header) return(0);

	MemHandle h2map = resource_map(header, data);

	return(h2map ? 0 : -1);
}
