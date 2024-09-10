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

	if(config.info.resource) {
		LOG_START("data_offset: 0x%04x", rre->data_offset);
		_LOG_(", file_offset: 0x%08x", offset);
		LOG_END(", size: 0x%04x", rdt.size);
	}

	return(&rdt);
}

static resource_reference_entry_p _resource_reference_entry_for_id(const uint16_t resID,
	resource_type_entry_p rte, resource_map_p map)
{
	PEDANTIC(assert(rte));
	PEDANTIC(assert(map));

	static resource_reference_entry_t rre;

	const uint32_t offset = map->header.map_offset
		+ map->type_list_offset
		+ rte->ref_list_offset;

	void* p = map->file_ref + offset;

	if(config.info.resource) {
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

		if(config.info.resource) {
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

	if(config.info.resource) {
		LOG_START("type: 0x%08x (%s)", type, (const char*)&type_string);
		LOG_END(", offset: 0x%08x, p: 0x%016" PRIxPTR, offset, (uintptr_t)p);
	}

	for(unsigned i = 0; i <= map->num_types; i++) {
		p = ldu32be(&rte.type, p);
		p = ldu16be(&rte.count, p);
		p = ldu16be(&rte.ref_list_offset, p);

		const uint32_t type_string[2] = { htole32(be32toh(rte.type)), 0 };

		if(config.info.resource) {
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

MemHandle resource_get(uint32_t type, uint16_t resID)
{
	resource_map_h h2map = current_resource;

	while(h2map) {
		MemHandle h = resource_get1(h2map, type, resID);
		if(h)
			return(h);

		h2map = h2map[0]->next;
	};

	return(0);
}

MemHandle resource_get1(resource_map_h h2map, uint32_t type, uint16_t resID)
{
	PEDANTIC(assert(h2map));

	if(!h2map) return(0);

	MemHandle rmh = 0;
	resource_map_p map = MemHandleLock((MemHandle)h2map);
	PEDANTIC(assert(map));

	if(!map) return(0);

	// find type list element
	resource_type_entry_p rte = _resource_type_entry_for_type(type, map);
	if(!rte) goto failExit;

	// get reference entry
	resource_reference_entry_p rre = _resource_reference_entry_for_id(resID, rte, map);
	if(!rte) goto failExit;

	// get resource data
	resource_data_p rd = _resource_data(rre, map);
	if(!rd) goto failExit;

	rmh = MemHandleNew(rd->size);
	PEDANTIC(assert(rmh));

	if(!rmh) goto failExit;

	MemPtr mhp = MemHandleLock(rmh);
	PEDANTIC(assert(mhp));

	if(mhp)
		memcpy(mhp, rd->data, rd->size);

failExit:
	MemHandleUnlock((MemHandle)h2map);

	return(rmh);
}

static resource_header_p resource_header(resource_header_p rh, void *const rf, const off_t offset)
{
	PEDANTIC(assert(rf));

	static resource_header_t srht;

	if(!rh) rh = &srht;

	void* p = rf + offset;

	p = ldu32be(&rh->data_offset, p);
	p = ldu32be(&rh->map_offset, p);
	p = ldu32be(&rh->data_length, p);
	p = ldu32be(&rh->map_length, p);

	if(config.info.resource) {
		LOG_START("data_offset: 0x%08x", rh->data_offset);
		_LOG_(", map_offset: 0x%08x", rh->map_offset);
		_LOG_(", data_length: 0x%08x", rh->data_length);
		LOG_END(", map_length: 0x%08x", rh->map_length);
	}

	return(rh);
}

static unsigned resource_header_verify_copy(resource_header_p const header, resource_header_p const copy)
{
	PEDANTIC(assert(header));
	PEDANTIC(assert(copy));
	PEDANTIC(assert(header != copy));

	const unsigned invalid = (header->data_length != copy->data_length)
		|| (header->data_offset != copy->data_offset)
		|| (header->map_length != copy->map_length)
		|| (header->map_offset != copy->map_offset);

	return(!invalid);
}

resource_map_h resource_map(resource_header_p const rh, void *const rf)
{
	PEDANTIC(assert(rh));
	PEDANTIC(assert(rf));

	if(!(rh && rf)) return(0);

	off_t map_offset = rh->map_offset;

	resource_header_p header_copy = resource_header(0, rf, map_offset);
	if(!resource_header_verify_copy(rh, header_copy))
		return(0);

	MemHandle h2map = MemHandleNew(sizeof(resource_map_t));
	PEDANTIC(assert(h2map));

	if(!h2map) return(0);

	resource_map_p map = MemHandleLock(h2map);

	memcpy(&map->header, rh, sizeof(resource_header_t));

	void* p = 16 + rf + map_offset;

	map->next = 0;
	p += 4;
	map->file_ref = rf;
	p += 2;

	/* **** */

	p = ldu16be(&map->attributes, p);
	p = ldu16be(&map->type_list_offset, p);
	p = ldu16be(&map->name_list_offset, p);
	p = ldu16be(&map->num_types, p);

	if(config.info.resource) {
		LOG_START("attrigbutes: 0x%04x", map->attributes);
		_LOG_(", type_list_offset: 0x%04x", map->type_list_offset);
		_LOG_(", name_list_offset: 0x%04x", map->name_list_offset);
		LOG_END(", num_types: %u", map->num_types);
	}

	/* **** */

	MemHandleUnlock(h2map);

	return((resource_map_h)h2map);
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

	resource_header_t rh, *p2rh = resource_header(&rh, data, 0);
	resource_map_h h2map = resource_map(p2rh, data);

	(*h2map)->next = current_resource;
	current_resource = h2map;

	return(0);
}
