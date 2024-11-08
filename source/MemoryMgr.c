#include "config.h"

#include "xMemoryMgr.h"

/* **** */

#include "chunk_t.h"
#include "utility.h"

/* **** */

#include "git/libbse/include/err_test.h"
#include "git/libbse/include/log.h"
#include "git/libbse/include/queue.h"
#include "git/libbse/include/unused.h"

/* **** */

#include <assert.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* **** */

static queue_t chunk_queue;
static queue_t master_pointer_block_queue;

/* **** */

struct config_memory_manager_t {
	struct {
		struct {
			unsigned entry:1;
			unsigned exit:1;
		}at;
	}trace;
	unsigned info:1;
}memory_manager_config;

#define TRACE memory_manager_config.trace

typedef struct queue_search_chunk_info_t* queue_search_chunk_info_p;
typedef queue_search_chunk_info_p const queue_search_chunk_info_ref;
typedef struct queue_search_chunk_info_t {
	union {
		void* data;
		MemHandle handle;
		MemPtr pointer;
		master_pointer_p master_pointer;
	};
//
	mem_chunk_p chunk;
	qelem_p lhs;
}queue_search_chunk_info_t;

typedef struct queue_search_handle_info_t* queue_search_handle_info_p;
typedef queue_search_handle_info_p const queue_search_handle_info_ref;
typedef struct queue_search_handle_info_t {
	union {
		void* data;
		MemHandle handle;
		master_pointer_p mp;
		MemPtr pointer;
	};
//
	unsigned index;
	master_pointer_block_p mpb;
	queue_iterator_search_fn sub_search_fn;
}queue_search_handle_info_t;

/* **** */

static master_pointer_block_p ___master_pointer_block_calloc(void)
{
	master_pointer_block_ref mpb = calloc(1, sizeof(master_pointer_block_t));
	ERR_NULL(mpb);

	if(!mpb) return(0);

	mpb->qnext.data = mpb;

	queue_enqueue(&mpb->qnext, &master_pointer_block_queue);

	return(mpb);
}

__attribute__((constructor))
static void ___memory_manager_config_init(void)
{
	(void)memset(&memory_manager_config, 0, sizeof(memory_manager_config));

//	memory_manager_config.info = 1;
//	memory_manager_config.trace.at.entry = 1;
//	memory_manager_config.trace.at.exit = 1;
}

__attribute__((constructor))
static void ___memory_manager_globals_init(void)
{
	AT_INIT(LOG());

	if(memory_manager_config.info) {
		LOGzu(sizeof(mem_chunk_t));
		LOGzu(sizeof(master_pointer_p));
		LOGzu(sizeof(master_pointer_block_t));
	}

	queue_init(&chunk_queue);
	queue_init(&master_pointer_block_queue);

	(void)___master_pointer_block_calloc();
}

static int __queue_iterator_search__chunk__from_handle(queue_iterator_search_ref qis, void *const param, void *const qdata)
{
	PEDANTIC(assert(qdata));
	PEDANTIC(assert(param));

	mem_chunk_ref mc = qdata;
	queue_search_chunk_info_ref csi = param;

	if(mc->master_pointer == csi->master_pointer) {
		csi->lhs = qis->qi.lhs;
		return(1);
	}

	return(0);
	UNUSED(qis);
}

static int __queue_iterator_search__chunk__from_ptr(queue_iterator_search_ref qis, void *const param, void *const qdata)
{
	PEDANTIC(assert(qdata));
	PEDANTIC(assert(param));

	mem_chunk_ref mc = qdata;
	queue_search_chunk_info_ref csi = param;

	if(&mc->data == csi->data) {
		csi->chunk = mc;
		csi->lhs = qis->qi.lhs;
		return(1);
	}

	return(0);
	UNUSED(qis);
}

static int __queue_iterator_search__mpb__from_handle(queue_iterator_search_ref qis, void *const param, void *const qdata)
{
	queue_search_handle_info_ref hsi = param;
	master_pointer_ref mp = qdata;

	int found = (!hsi->handle && !mp->flags.alloc)
		| (mp == hsi->mp);

	return(found);
	UNUSED(qis);
}

static int __queue_iterator_search__mpb__from_ptr(queue_iterator_search_ref qis, void *const param, void *const qdata)
{
	queue_search_handle_info_ref hsi = param;
	master_pointer_ref mp = qdata;

	return(mp == hsi->data);
	UNUSED(qis);
}

static int __queue_iterator_search__mpb(queue_iterator_search_ref qis, void *const param, void *const qdata)
{
	PEDANTIC(assert(qdata));
	PEDANTIC(assert(param));

	queue_search_handle_info_ref hsi = param;
	master_pointer_block_ref mpb = qdata;

	if(0 && memory_manager_config.trace.at.entry) {
		LOG_START(">>");
		_LOG_(" mpb: 0x%016" PRIxPTR, (uintptr_t)mpb);
		LOG_END(", hsi: 0x%016" PRIxPTR, (uintptr_t)hsi);
	}

	master_pointer_p mp = 0;

	unsigned index = 0;
	for(; index < kMasterPointerCount; index++) {
		mp = &mpb->master_pointer[index];

		if(0 && memory_manager_config.info) {
			LOG_START(">>");
			_LOG_("[0x%02x]", index);
			_LOG_("mp: 0x%016" PRIxPTR "->(", (uintptr_t)mp);
			_LOG_("chunk: 0x%016" PRIxPTR, (uintptr_t)mp->chunk);
			_LOG_("data: 0x%016" PRIxPTR ")", (uintptr_t)mp->data);
			LOG_END();
		}

		if(hsi->sub_search_fn(qis, param, mp))
			goto exit_found;
	}

	if(0 && memory_manager_config.trace.at.exit) {
		LOG("<<");
	}

	return(0);

exit_found:
	hsi->index = index;
	hsi->mp = mp;
	hsi->mpb = mpb;

	if(memory_manager_config.trace.at.exit) {
		LOG_START("<<");
		_LOG_(" index: 0x%02x", index);
		_LOG_(", mp: 0x%016" PRIxPTR, (uintptr_t)mp);
		LOG_END("->data: 0x%016" PRIxPTR, (uintptr_t)mp->data);
	}

	return(1);
	UNUSED(qis);
}

/* **** */

static size_t __mem_chunk__sizeof(const size_t size)
{ return(sizeof(mem_chunk_t) + size); }

/* **** */

static size_t __mem_chunk_sizeof_alloc(const size_t size)
{ return((3 + __mem_chunk__sizeof(size)) & ~3); }

/* **** */

static int __queue_insert_sorted_chunk(qelem_ref lhs, qelem_ref insert, qelem_ref rhs)
{
	if(rhs < insert)
		return(QSORT_CONTINUE);

	return(QSORT_INSERT);
	UNUSED(lhs);
}

/* **** */

static master_pointer_p _master_pointer__search(queue_search_handle_info_ref hsi, const unsigned validate)
{
	queue_iterator_search_t qis;
	queue_iterator_search_init(&qis, &master_pointer_block_queue, __queue_iterator_search__mpb);

	master_pointer_p mp = 0;

	if(1 == queue_iterator_search(&qis, hsi)) {
		mp = hsi->mp;

		if(validate) {
			DEBUG(MasterPointerValidate(mp, 0));
		}
	}

	return(mp);
}

static master_pointer_p _master_pointer__search__from_handle(queue_search_handle_info_ref hsi, MemHandle const h)
{
	hsi->handle = h;
	hsi->sub_search_fn = __queue_iterator_search__mpb__from_handle;

	return(_master_pointer__search(hsi, !!h));
}

static master_pointer_p _master_pointer__search__from_pointer(queue_search_handle_info_ref hsi, MemPtr const p)
{
	hsi->pointer = p;
	hsi->sub_search_fn = __queue_iterator_search__mpb__from_ptr;

	return(_master_pointer__search(hsi, true));
}

static Boolean _master_pointer__validate(master_pointer_ref mp, mem_chunk_ref mcP)
{
	assert(mp);
	assert(mp->chunk);
	assert(mp->data);
	assert(mp->flags.alloc);

	if(mcP) {
		assert(mcP == mp->chunk);
	}

	mem_chunk_ref mc = mp->chunk;

	assert(mp == mc->master_pointer);
	assert(&mc->data == mp->data);

	return(1);
}


static master_pointer_p _master_pointer_alloc(master_pointer_block_href h2mpb)
{
	queue_search_handle_info_t hsi;
	master_pointer_p mp = _master_pointer__search__from_handle(&hsi, 0);

	master_pointer_block_p mpb = hsi.mpb;

	if(!mp) {
		mpb = ___master_pointer_block_calloc();
		mp = mpb ? &mpb->master_pointer[0] : 0;
	}

	if(mp)
		mp->flags.alloc = 1;

	if(h2mpb) *h2mpb = mp ? mpb : 0;

	return(mp);
}

static void _master_pointer_free(master_pointer_ref mp, master_pointer_block_ref mpb)
{
	assert(mp);

	memset(mp, 0, sizeof(master_pointer_t));

	assert(!mpb); // TODO
}

static mem_chunk_p _mem_chunk__calloc(const size_t size, const unsigned is_movable)
{
	mem_chunk_ref mc = calloc(1, __mem_chunk_sizeof_alloc(size));
	ERR_NULL(mc);

	if(memory_manager_config.info) {
		LOG_START("alloc_size: 0x%08zx", size);
		_LOG_(", chunk_size: 0x%08zx", __mem_chunk__sizeof(size));
		LOG_END(", chunk_alloc: 0x%08zx", __mem_chunk_sizeof_alloc(size));
		LOG("mem_chunk_p: 0x%016" PRIxPTR, (uintptr_t)mc);
	}

	if(!mc) LOG_ACTION(return(0));

	qelem_ref cqe = &mc->qnext;
	cqe->data = mc;

	mc->alloc = __mem_chunk_sizeof_alloc(size);
	mc->size = size;

	if(memory_manager_config.info) {
		LOG_START("qHead: 0x%016" PRIxPTR, (uintptr_t)chunk_queue.head);
		LOG_END(", qTail: 0x%016" PRIxPTR, (uintptr_t)chunk_queue.tail);
	}

	queue_insert_sorted(cqe, &chunk_queue, __queue_insert_sorted_chunk);

	if(memory_manager_config.info) {
		LOG_START("qHead: 0x%016" PRIxPTR, (uintptr_t)chunk_queue.head);
		LOG_END(", qTail: 0x%016" PRIxPTR, (uintptr_t)chunk_queue.tail);
	}

	return(mc);
	UNUSED(is_movable); // TODO
}

static mem_chunk_p _mem_chunk__search__from_handle(queue_search_chunk_info_ref csi, MemHandle const h, Boolean need_lhs)
{
	if(memory_manager_config.trace.at.entry)
		LOG(">> h: 0x%016" PRIxPTR, (uintptr_t)h);

	mem_chunk_p mc = 0;
	master_pointer_p mp = 0;

	if(need_lhs) {
		csi->chunk = 0;
		csi->handle = h;

		queue_iterator_search_t qis;
		queue_iterator_search_init(&qis, &chunk_queue, __queue_iterator_search__chunk__from_handle);
		if(1 == queue_iterator_search(&qis, csi)) {
			mc = csi->chunk;
			mp = mc ? mc->master_pointer : 0;
		}
	} else {
		queue_search_handle_info_t hsi;
		mp = _master_pointer__search__from_handle(&hsi, h);
		mc = mp ? mp->chunk : 0;
		csi->chunk = mc;
	}

	if(mc && mp) {
		DEBUG(MemChunkValidate(mc, 0, mp));
		DEBUG(MasterPointerValidate(mp, mc));
	}

	return(mc);
}

static mem_chunk_p _mem_chunk__search__from_ptr(queue_search_chunk_info_ref csi, MemPtr const p)
{
	PEDANTIC(assert(csi));

	if(memory_manager_config.trace.at.entry)
		LOG(">> p: 0x%016" PRIxPTR, (uintptr_t)p);

	queue_iterator_search_t qis;

	csi->chunk = 0;
	csi->pointer = p;

	queue_iterator_search_init(&qis, &chunk_queue, __queue_iterator_search__chunk__from_ptr);

	int found  = (1 == queue_iterator_search_next(&qis, csi));
	mem_chunk_ref mc = found ? csi->chunk : 0;

	if(!found) {
		LOG("not pointer?");

		if(MemPtrRecoverHandle(p)) {
			LOG("lost chunk?");
		}

		PEDANTIC(assert(mc));
	}

	DEBUG(MemChunkValidate(mc, p, 0));

	if(memory_manager_config.trace.at.exit)
		LOG("<< mc: 0x%016" PRIxPTR, (uintptr_t)mc);

	return(mc);
}

static Boolean _mem_chunk__validate(mem_chunk_ref mc, MemPtr const p, master_pointer_ref mP)
{
	assert(mc);

	if(mc->alloc < mc->size) {
		LOG_START("mc->(");
		_LOG_("alloc: 0x%08zx", mc->alloc);
		_LOG_(", size: 0x%08zx", mc->size);
		LOG_END(")");
	}

	assert(mc->alloc >= mc->size);
	assert(!(mc->alloc & 3));
	assert(0 == mc->free);
	assert(0 == mc->heap);

	if(p) {
		void *const data = &mc->data;

		if(p != data) {
			LOG_START("p: 0x%016" PRIxPTR, (uintptr_t)p);
			LOG_END(", data: 0x%016" PRIxPTR, (uintptr_t)data);
		}

		assert(p == data);
	}

	if(mP)
		assert(mP == mc->master_pointer);

	return(1);
}

static void _mem_chunk_free(mem_chunk_ref mc, qelem_ref lhs)
{
	PEDANTIC(assert(mc));
	PEDANTIC(assert(!mc->free));

	if(!mc) return;
	if(mc->free) return;

//	mc->alloc = 0;
	mc->free = 1;

	if(mc->master_pointer)
		_master_pointer_free(mc->master_pointer, 0);

	if(queue_dequeue(&mc->qnext, lhs, &chunk_queue)) {
		PEDANTIC(memset(mc, 0, mc->alloc));
		free(mc);
	}
}

static void* _mem_chunk_lock(mem_chunk_ref mc)
{
	void *const data = mc ? &mc->data : 0;

	if(memory_manager_config.trace.at.entry) {
		LOG_START(">>");
		_LOG_(" mc: 0x%016" PRIxPTR, (uintptr_t)mc);
		LOG_END("->data: 0x%016" PRIxPTR, (uintptr_t)data);
	}

	PEDANTIC(assert(mc));
	PEDANTIC(assert(15 != mc->lock));

	const unsigned lock = mc->lock < 14;
	const unsigned was = mc->lock;

	mc->lock += lock;

	if(memory_manager_config.info) {
		LOG_START("data: 0x%016" PRIxPTR, (uintptr_t)data);
		LOG_END(", mc->lock: ((%u)%s --> %u)", was, lock ? "++" : "", mc->lock);
	}

	if(memory_manager_config.trace.at.exit)
		LOG("<<");

	return(lock ? data : 0);
}

static UInt32 _mem_chunk_size(mem_chunk_ref mc)
{
	if(!mc) return(0);

	return(mc->size);
}

static Err _mem_chunk_unlock(mem_chunk_ref mc)
{
	if(0 && memory_manager_config.trace.at.entry) {
		LOG_START(">>");
		_LOG_(" mc: 0x%016" PRIxPTR, (uintptr_t)mc);
		if(mc) _LOG_("->lock: %u", mc->lock);
		LOG_END();
	}

	PEDANTIC(assert(mc));
	PEDANTIC(assert(15 > mc->lock));

	const unsigned unlock = mc->lock && (15 > mc->lock);
	const unsigned was = mc->lock;

	mc->lock -= unlock;

	if(memory_manager_config.info) {
		LOG_START("data: 0x%016" PRIxPTR, (uintptr_t)&mc->data);
		LOG_END(", mc->lock: ((%u)%s --> %u)", was, unlock ? "--" : "", mc->lock);
	}

	if(memory_manager_config.trace.at.exit) LOG("<<");

	return(errNone);
}

/* **** */

int mem_handle_resource(MemHandle const h, const unsigned set_resource, const unsigned value)
{
	int is_resource = -1;

	if(memory_manager_config.trace.at.entry) {
		LOG_START(">> h: 0x%016" PRIxPTR, (uintptr_t)h);
		LOG_END(", set_resource: 0x%016" PRIxPTR, (uintptr_t)set_resource);
	}

	PEDANTIC(assert(h));

	if(!h) goto exit_trace;

	queue_search_handle_info_t hsi;
	master_pointer_ref mp = _master_pointer__search__from_handle(&hsi, h);
	mem_chunk_ref mc = mp ? mp->chunk : 0;

	if(!mc) goto exit_trace;

	is_resource = mc->resource;

	if(set_resource)
		mc->resource = value;

exit_trace:
	if(memory_manager_config.trace.at.exit) LOG("<<");

	return(is_resource);
}

/* **** */


Boolean MasterPointerValidate(master_pointer_ref mp, mem_chunk_ref mc)
{
	assert(mp);

	Boolean valid = !!_master_pointer__validate(mp, mc);
	valid &= !!_mem_chunk__validate(mp->chunk, mp->data, mp);

	return(1);
}

Boolean MemChunkValidate(mem_chunk_ref mc, MemPtr const p, master_pointer_ref mp)
{
	assert(mc);

	Boolean valid = !!_mem_chunk__validate(mc, p, mp);

	if(mp) {
		assert(mp == mc->master_pointer);
		valid &= !!_master_pointer__validate(mp, mc);
	} else if(mc->master_pointer)
		valid &= !!_master_pointer__validate(mc->master_pointer, mc);

	return(1);
}

Err MemHandleFree(MemHandle const h)
{
	if(memory_manager_config.trace.at.entry) {
		LOG(">> h: 0x%016" PRIxPTR, (uintptr_t)h);
	}

	PEDANTIC(assert(h));

	queue_search_chunk_info_t csi;
	mem_chunk_ref mc = _mem_chunk__search__from_handle(&csi, h, true);
	master_pointer_ref mp = mc ? mc->master_pointer : 0;

	if(!mc)
		return(memErrInvalidParam);

	if(0 == mc->lock) {
		_mem_chunk_free(mc, csi.lhs);
		_master_pointer_free(mp, 0);
	} else if (0) {
		LOG_START("0x%016" PRIxPTR, (uintptr_t)h);
		_LOG_("->0x%016" PRIxPTR, (uintptr_t)mc);
		LOG_END( " -- locked");
	}

	return(errNone);
}

MemPtr MemHandleLock(MemHandle const h)
{
	if(memory_manager_config.trace.at.entry) {
		LOG(">> h: 0x%016" PRIxPTR, (uintptr_t)h);
	}

	PEDANTIC(assert(h));

	queue_search_handle_info_t hsi;
	master_pointer_ref mp = _master_pointer__search__from_handle(&hsi, h);
	mem_chunk_ref mc = mp ? mp->chunk : 0;

	if(!mc)
		LOG_ACTION(return(0));

	return(_mem_chunk_lock(mc));
}

MemHandle MemHandleNew(const UInt32 size)
{
	if(memory_manager_config.trace.at.entry) {
		LOG(">> size: 0x%08lx", size);
	}

	master_pointer_block_p mpb = 0;
	master_pointer_ref mp = _master_pointer_alloc(&mpb);
	PEDANTIC(assert(mp));

	if(!mp) LOG_ACTION(return(0));

	mem_chunk_ref mc = _mem_chunk__calloc(size, 1);
	PEDANTIC(assert(mc));

	if(!mc) {
		_master_pointer_free(mp, mpb);
		LOG_ACTION(return(0));
	}

	void *const data = mc ? &mc->data : 0;

	if(memory_manager_config.info) {
		LOG_START("master_pointer: 0x%016" PRIxPTR, (uintptr_t)mp);
		_LOG_(", mem_chunk_p: 0x%016" PRIxPTR, (uintptr_t)mc);
		_LOG_(", qelem_p: 0x%016" PRIxPTR, (uintptr_t)&mc->qnext);
		LOG_END(", data: 0x%016" PRIxPTR, (uintptr_t)data);
	}

	mc->master_pointer = mp;

	mp->chunk = mc;
	mp->data = data;

	DEBUG(MasterPointerValidate(mp, mc));
	DEBUG(MemChunkValidate(mc, data, mp));

	queue_search_chunk_info_t csi;
	assert(mc == _mem_chunk__search__from_handle(&csi, mc->handle, false));
	assert(mc == _mem_chunk__search__from_ptr(&csi, data));

	return(mc->handle);
}

UInt32 MemHandleSize(MemHandle const h)
{
	queue_search_handle_info_t hsi;
	master_pointer_ref mp = _master_pointer__search__from_handle(&hsi, h);
	mem_chunk_ref mc = mp ? mp->chunk : 0;

	return(_mem_chunk_size(mc));
}

Err MemHandleUnlock(MemHandle const h)
{
	if(memory_manager_config.trace.at.entry) {
		LOG(">> h: 0x%016" PRIxPTR, (uintptr_t)h);
	}

	PEDANTIC(assert(h));

	queue_search_handle_info_t hsi;
	master_pointer_ref mp = _master_pointer__search__from_handle(&hsi, h);
	mem_chunk_ref mc = mp ? mp->chunk : 0;

	if(!mc)
		return(memErrInvalidParam);

	return(_mem_chunk_unlock(mc));
}

Err MemMove(void *const dst, const void* src, const Int32 n)
{
	if(memory_manager_config.trace.at.entry) {
		LOG_START(">>");
		_LOG_(" dst: 0x%016" PRIxPTR, (uintptr_t)dst);
		_LOG_(" src: 0x%016" PRIxPTR, (uintptr_t)src);
		LOG_END(" n: 0x%08lx", n);
	}

	LOG();
	PEDANTIC(assert(dst));
	PEDANTIC(assert(src));

	memmove(dst, src, n); return(0);
}

Err MemPtrFree(const MemPtr p)
{
	if(memory_manager_config.trace.at.entry) {
		LOG(">> p: 0x%016" PRIxPTR, (uintptr_t)p);
	}

	queue_search_chunk_info_t csi;
	mem_chunk_ref mc = _mem_chunk__search__from_ptr(&csi, p);
	if(!mc) return(memErrInvalidParam);

	_mem_chunk_free(mc, csi.lhs);
	return(errNone);
}

MemPtr MemPtrNew(const UInt32 size)
{
	if(memory_manager_config.trace.at.entry) {
		LOG(">> size: 0x%08lx", size);
	}

	mem_chunk_ref mc = _mem_chunk__calloc(size, false);
	void *const data = mc ? &mc->data : 0;

	DEBUG(MemChunkValidate(mc, data, 0));

	return(data);
}

MemPtr MemPtrNewClear(const size_t size)
{
	MemPtr p = MemPtrNew(size);
	if(p)
		memset(p, 0, size);

	return(p);
}

MemHandle MemPtrRecoverHandle(MemPtr const p)
{
	if(memory_manager_config.trace.at.entry) {
		LOG(">> p: 0x%016" PRIxPTR, (uintptr_t)p);
	}

	queue_search_handle_info_t hsi;
	master_pointer_ref mp = _master_pointer__search__from_pointer(&hsi, p);

	if(mp) {
		mem_chunk_ref mc = mp->chunk;

		LOG_START("mpb: 0x%016" PRIxPTR, (uintptr_t)hsi.mpb);
		_LOG_("[%u]", hsi.index);
		LOG_END(".mp: 0x%016" PRIxPTR, (uintptr_t)mp);
		LOG_START("(data: 0x%016" PRIxPTR, (uintptr_t)mp->data);
		LOG_END(", chunk: 0x%016" PRIxPTR ")", (uintptr_t)mc);

		return((MemHandle)mp);
	}

	return(0);
}

UInt32 MemPtrSize(MemPtr const p)
{
	if(memory_manager_config.trace.at.entry) {
		LOG(">> p: 0x%016" PRIxPTR, (uintptr_t)p);
	}

	queue_search_chunk_info_t csi;
	mem_chunk_ref mc = _mem_chunk__search__from_ptr(&csi, p);

	return(_mem_chunk_size(mc));
}

Err MemPtrUnlock(MemPtr const p)
{
	if(memory_manager_config.trace.at.entry) {
		LOG(">> p: 0x%016" PRIxPTR, (uintptr_t)p);
	}

	PEDANTIC(assert(p));

	queue_search_chunk_info_t csi;
	mem_chunk_ref mc = _mem_chunk__search__from_ptr(&csi, p);

	if(!mc)
		return(memErrInvalidParam);

	return(_mem_chunk_unlock(mc));
}

Boolean MemValidateHandle(MemHandle const h)
{
	TRACE_ENTRY("h: 0x%016" PRIxPTR, (uintptr_t)h);

	PEDANTIC(assert(h));

	queue_search_handle_info_t hsi;
	master_pointer_ref mp = _master_pointer__search__from_handle(&hsi, h);
	mem_chunk_ref mc = mp ? mp->chunk : 0;

	Boolean valid = _master_pointer__validate(mp, mc)
		&& _mem_chunk__validate(mc, 0, mp);

	TRACE_EXIT();
	return(valid);
}
