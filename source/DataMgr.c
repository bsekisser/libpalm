#include "DataMgr.h"

/* **** */

#include "dm_t.h"

/* **** */

#include "git/libbse/include/log.h"
//#include "git/libbse/include/unused.h"

/* **** */

static Err __DmLastErr;
static Char* __mem_card; // TODO

/* **** */

Boolean __DmDatabaseAttr(dmdb_p const db, const UInt16 attribute, Boolean *const p2set)
{
	Boolean wasSet = db->attributes & attribute;

	if(p2set) {
		if(*p2set)
			db->attributes |= attribute;
		else
			db->attributes &= ~attribute;
	}

	return(wasSet);
}

dmdb_p __DmOpenDatabase(UInt16 card, LocalID dbID)
{ return((dmdb_p)(__mem_card[card] + dbID)); }

Boolean __DmRecordAttr(dmdbrecord_p const dbr, const UInt16 attribute, Boolean *const p2set)
{
	Boolean wasSet = dbr->attributes & attribute;

	if(p2set) {
		if(*p2set)
			dbr->attributes |= attribute;
		else
			dbr->attributes &= ~attribute;
	}

	return(wasSet);
}

dmdbrecord_p __DmGetRecord(DmOpenRef dbP, UInt16 index)
{
	dmdbrecord_p2h dbr_l1 = ((dmdb_p)dbP)->record[index >> 8];
	if(!dbr_l1)
		return(__DmSetLastErr(dmErrIndexOutOfRange));

	dmdbrecord_h dbr = dbr_l1[index & 0xff];
	if(!dbr_l1)
		return(__DmSetLastErr(dmErrIndexOutOfRange));

	return(*dbr);
}

void* __DmSetLastErr(Err err)
{
	__DmLastErr = err;
	return(0);
}

Boolean __DmSetDatabaseAttr(dmdb_p const dbr, const UInt16 attribute, const Boolean set)
{ return(__DmDatabaseAttr(dbr, attribute, (Boolean*)&set)); }

Boolean __DmSetRecordAttr(dmdbrecord_p const dbr, const UInt16 attribute, const Boolean set)
{ return(__DmRecordAttr(dbr, attribute, (Boolean*)&set)); }
