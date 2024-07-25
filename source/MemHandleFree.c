#include "MemoryMgr.h"

/* **** */

#include "chunk_t.h"
#include "utility.h"

/* **** */

#include "libbse/include/log.h"
#include "libbse/include/page.h"

/* **** */

#include <stdlib.h>

/* **** */

Err MemHandleFree(MemHandle h)
{
	chunk_p chunk = __MemHandle2Chunk(h);

	if(!chunk->isHandle)
		return(-1);

	free(chunk);

	return(errNone);
}
