#include "xMemoryMgr.h"

/* **** */

MemHandle MemHandleNew(UInt32 size)
{
	master_pointer_p mp = master_pointer_calloc(size);

	mp->size = size;

	return((MemHandle)mp);
}
