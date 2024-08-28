#include "xList.h"

/* **** */

#include "libbse/include/log.h"
//#include "libbse/include/unused.h"

/* **** */

Int16 LstGetSelection(const ListType* listP)
{ return(listP->currentItem); }

void LstSetSelection(ListType* listP, Int16 itemNum)
{
	LOG("TODO");

	if(itemNum < listP->topItem)
		return;
	if(itemNum > listP->numItems)
		return;

	listP->currentItem = itemNum;
}
