#include "xMemoryMgr.h"

/* **** */

#include <string.h>

/* **** */

Err MemHandleFree(MemHandle h)
{
	master_pointer_p mp = master_pointer_find_handle(h, 0);
	if(!mp)
		return(memErrInvalidParam);

	if(mp->alloc)
		free(mp->data);

	memset(mp, 0, sizeof(master_pointer_t));

	return(errNone);
}
