#include "config.h"

#include "xList.h"

/* **** */

#include "libbse/include/log.h"
//#include "libbse/include/unused.h"

/* **** */

#include <assert.h>

/* **** */

Int16 LstGetSelection(const ListType* listP)
{
	PEDANTIC(assert(listP));

	return(listP ? listP->currentItem : noListSelection);
}

void LstSetSelection(ListType* listP, Int16 itemNum)
{
	PEDANTIC(assert(listP));

	LOG("TODO");

	if(!listP) return;

	if(itemNum < listP->topItem)
		return;
	if(itemNum > listP->numItems)
		return;

	listP->currentItem = itemNum;
}
