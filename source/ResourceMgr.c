#include "xMemoryMgr.h"
#include "xResourceMgr.h"

/* **** */

#include "libbse/include/err_test.h"
#include "libbse/include/log.h"
#include "libbse/include/mem_access_be.h"
#include "libbse/include/mem_access_le.h"

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

static uint8_t __uint8(void** p2p)
{ return(*(uint8_t*)p2p[0]++); }

static uint32_t __uint16be(void** p2p)
{
	void *const p = *p2p;
	(*p2p) += 2;

	return(mem_16be_access(p, 0));
}

static uint32_t __uint24be(void** p2p)
{
	void *const p = *p2p;
	(*p2p) += 3;

	return(mem_access_be(p, 3, 0));
}

static uint32_t __uint32be(void** p2p)
{
	void *const p = *p2p;
	(*p2p) += 4;

	return(mem_32be_access(p, 0));
}

static uint32_t __uint32le(void** p2p)
{
	void *const p = *p2p;
	(*p2p) += 4;

	return(mem_32le_access(p, 0));
}

/* **** */

static resource_data_p _resource_data(resource_reference_entry_p const rre, resource_map_p const map)
{
	static resource_data_t rdt;

	const off_t offset = map->header_copy.data_offset
		+ rre->data_offset;

	void* p = map->file_ref + offset;

	rdt.size = __uint32be(&p);
	rdt.data = p;

	LOG_START("data_offset: 0x%04lx", rre->data_offset);
	_LOG_(", file_offset: 0x%08lx", offset);
	LOG_END(", size: 0x%04zx", rdt.size);

	return(&rdt);
}

static resource_reference_entry_p _resource_reference_entry_for_id(const uint16_t resID,
	resource_type_entry_p rte, resource_map_p map)
{
	static resource_reference_entry_t rre;

	const off_t offset = map->header_copy.map_offset
		+ map->type_list_offset
		+ rte->ref_list_offset;

	void* p = map->file_ref + offset;

	LOG_START("id: %u", resID);
	LOG_END(", offset: 0x%08lx, p: 0x%016" PRIxPTR, offset, (uintptr_t)p);

	for(unsigned i = 0; i <= rte->count; i++) {
		rre.id = __uint16be(&p);
		rre.name_offset = __uint16be(&p);
		rre.attributes = __uint8(&p);
		rre.data_offset = __uint24be(&p);
		p += 4; // reserved for handle to resource

		if(1) {
			LOG_START("id: %u", rre.id);
			_LOG_(", name_offset: 0x%04lx", rre.name_offset);
			_LOG_(", attributes: 0x%02x", rre.attributes);
			LOG_END(", data_offset: 0x%06lx", rre.data_offset);
		}

		if(resID == rre.id)
			return(&rre);
	}

	return(0);
}

static resource_type_entry_p _resource_type_entry_for_type(const uint32_t type, resource_map_p const map)
{
	static resource_type_entry_t rte;

	const off_t offset = 2 + map->header_copy.map_offset
		+ map->type_list_offset;

	void* p = map->file_ref + offset;

	const uint32_t type_string[2] = { htole32(be32toh(type)), 0 };
	LOG_START("type: 0x%08x (%s)", type, (const char*)&type_string);
	LOG_END(", offset: 0x%08lx, p: 0x%016" PRIxPTR, offset, (uintptr_t)p);

	for(unsigned i = 0; i <= map->num_types; i++) {
		rte.type = __uint32be(&p);
		rte.count = __uint16be(&p);
		rte.ref_list_offset = __uint16be(&p);

		const uint32_t type_string[2] = { htole32(be32toh(rte.type)), 0 };

		if(1) {
			LOG_START("type: 0x%08x (%s)", rte.type, (const char*)&type_string);
			_LOG_(", count: 0x%04x", rte.count);
			LOG_END(", ref_list_offset: 0x%04lx", rte.ref_list_offset);
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
	resource_map_h h2map = libpalm->current_resource;

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
	assert(h2map);

	MemHandle rmh = 0;
	resource_map_p map = MemHandleLock((MemHandle)h2map);
	assert(map);

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
	MemPtr mhp = MemHandleLock(rmh);

	memcpy(mhp, rd->data, rd->size);

failExit:
	MemHandleUnlock((MemHandle)h2map);

	return(rmh);
}

static resource_header_p resource_header(resource_header_p rh, void *const rf, const off_t offset)
{
	static resource_header_t srht;

	if(!rh) rh = &srht;

	void* p = rf + offset;

	rh->data_offset = __uint32be(&p);
	rh->map_offset = __uint32be(&p);
	rh->data_length = __uint32be(&p);
	rh->map_length = __uint32be(&p);

	if(1) {
		LOG_START("data_offset: 0x%08lx", rh->data_offset);
		_LOG_(", map_offset: 0x%08lx", rh->map_offset);
		_LOG_(", data_length: 0x%08zx", rh->data_length);
		LOG_END(", map_length: 0x%08zx", rh->map_length);
	}


	return(rh);
}

static unsigned resource_header_verify_copy(resource_header_p const header, resource_header_p const copy)
{
	const unsigned invalid = (header->data_length != copy->data_length)
		|| (header->data_offset != copy->data_offset)
		|| (header->map_length != copy->map_length)
		|| (header->map_offset != copy->map_offset);

	return(!invalid);
}

resource_map_h resource_map(resource_header_p const rh, void *const rf)
{
	off_t map_offset = rh->map_offset;

	resource_header_p header_copy = resource_header(0, rf, map_offset);
	if(!resource_header_verify_copy(rh, header_copy))
		return(0);

	MemHandle h2map = MemHandleNew(sizeof(resource_map_t));
	resource_map_p map = MemHandleLock(h2map);

	memcpy(&map->header_copy, header_copy, sizeof(resource_header_t));

	void* p = 16 + rf + map_offset;

	map->next = 0;
	p += 4;
	map->file_ref = rf;
	p += 2;

	/* **** */

	map->attributes = __uint16be(&p);
	map->type_list_offset = __uint16be(&p);
	map->name_list_offset = __uint16be(&p);
	map->num_types = __uint16be(&p);

	if(1) {
		LOG_START("attrigbutes: 0x%04x", map->attributes);
		_LOG_(", type_list_offset: 0x%04lx", map->type_list_offset);
		_LOG_(", name_list_offset: 0x%04lx", map->name_list_offset);
		LOG_END(", num_types: %u", map->num_types);
	}

	/* **** */

	MemHandleUnlock(h2map);

	return((resource_map_h)h2map);
}

int resource_open(const char* path)
{
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

	(*h2map)->next = libpalm->current_resource;
	libpalm->current_resource = h2map;

	return(0);
}
