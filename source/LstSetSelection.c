#include "xList.h"

/* **** */

#include "libbse/include/log.h"
//#include "libbse/include/unused.h"

/* **** */

void LstSetSelection(ListType* listP, Int16 itemNum)
{
	if(itemNum < listP->topItem)
		return;
	if(itemNum > listP->numItems)
		return;

	listP->currentItem = itemNum;
	LOG("TODO");
}
