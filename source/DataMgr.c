#include "sdk/include/Core/System/DataMgr.h"

/* **** */

#include "dm_t.h"
#include "utility.h"
#include "xResourceMgr.h"

/* **** */

#include "git/libbse/include/log.h"
#include "git/libbse/include/unused.h"

/* **** */

#include <string.h>

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

/* **** */

Err DmCloseDatabase(DmOpenRef dbP)
{
	LOG("TODO"); return(0);

	dmdb_p db = dbP;

	if(db->attributes & dmHdrAttrRecyclable)
		DmDeleteDatabase(db->cardNo, db->dbID);
}

Err DmCreateDatabase(UInt16 cardNo, const Char* nameP, UInt32 creator, UInt32 type, Boolean resDB)
{
	LOG_START("cardNo: %u", cardNo);

	if(nameP)
		_LOG_(", name: %s", nameP);

	const char* cc = (char*)&creator;
	_LOG_(", creator: %c%c%c%c", cc[0], cc[1], cc[2], cc[3]);

	const char* tt = (char*)type;
	_LOG_(", type: %c%c%c%c", tt[0], tt[1], tt[2], tt[3]);

	LOG_END(", resDB: %u", resDB)

	LOG("TODO"); return(0);
}

Err DmDatabaseInfo(const UInt16 cardNo, const LocalID dbID,
	Char *const nameP,
	UInt16 *const attributesP,
	UInt16 *const versionP,
	UInt32 *const crDateP,
	UInt32 *const modDateP,
	UInt32 *const bckUpDateP,
	UInt32 *const modNumP,
	LocalID *const appInfoIDP,
	LocalID *const sortInfoIDP,
	UInt32 *const typeP,
	UInt32 *const creatorP)
{
	dmdb_p db = __DmOpenDatabase(cardNo, dbID);

	PGET(appInfoIDP, db->appInfoID);
	PGET(attributesP, db->attributes);
	PGET(bckUpDateP, db->bckUpDate);
	PGET(crDateP, db->crDate);
	PGET(creatorP, db->creator);
	PGET(modDateP, db->modDate);
	PGET(modNumP, db->modNum);

	if(nameP)
		strncpy(nameP, db->name, sizeof(db->name) - 1);

	PGET(sortInfoIDP, db->sortInfoID);
	PGET(typeP, db->type);
	PGET(versionP, db->version);

	return(errNone); // TODO -- other errors may be returned
}

LocalID DmFindDatabase(UInt16 cardNo, const Char* nameP)
{
	if(nameP)
		LOG("cardNo: %u, name: %s", cardNo, nameP);

	LOG("TODO"); return(0);
}

MemHandle DmGet1Resource(DmResType type, DmResID resID)
{ return((MemHandle)resource_get1(current_resource, type, resID)); }

Err DmGetNextDatabaseByTypeCreator(const Boolean newSearch, DmSearchStatePtr const stateInfoP,
	const UInt32 type, const UInt32 creator, const Boolean onlyLatestVers,
	UInt16 *const cardNoP, LocalID *const dbIDP)
{
	LOG("TODO"); return (-1);

	UNUSED(newSearch, stateInfoP, type, creator, onlyLatestVers, cardNoP, dbIDP);
}

MemHandle DmGetRecord(DmOpenRef dbP, UInt16 index)
{
	dmdbrecord_p dbr = __DmGetRecord(dbP, index);
	if(!dbr)
		return(0);

	Boolean busyWasSet = __DmSetRecordBusy(dbr, true);
	if(busyWasSet)
		return((MemHandle)__DmSetLastErr(dmErrRecordBusy));

	return((MemHandle)dbr);
}

MemHandle DmGetResource(DmResType type, DmResID resID)
{ return((MemHandle)resource_get(type, resID)); }

MemHandle DmNewRecord(DmOpenRef const dbP, UInt16 *const atP, const UInt32 size)
{
	MemHandle h2dbr = MemHandleNew(sizeof(dmdbrecord_t) + size);

	LOG("TODO");

	dmdbrecord_p dbr = *(dmdbrecord_h)h2dbr;

	dbr->p2db = dbP;

	__DmSetRecordBusy(dbr, true);
	__DmSetRecordDirty(dbr, true);

	return(h2dbr);

	UNUSED(atP);
}

DmOpenRef DmOpenDatabase(UInt16 cardNo, LocalID dbID, UInt16 mode)
{
	dmdb_p db = __DmOpenDatabase(cardNo, dbID);

	__DmSetDatabaseAttr(db, dmHdrAttrOpen, true);

	db->mode = mode;
	return(db);
}

Err DmOpenDatabaseInfo(DmOpenRef dbP,
	LocalID *const dbIDP,
	UInt16 *const openCountP,
	UInt16 *const modeP,
	UInt16 *const cardNoP,
	Boolean *const resDBP)
{
	if(!dbP) return(-1);

	dmdb_p db = (void*)dbP;

	PGET(cardNoP, db->cardNo);
	PGET(dbIDP, db->dbID);
	PGET(modeP, db->mode);
	PGET(openCountP, db->openCount);
	PGET(resDBP, !!(db->attributes & dmHdrAttrResDB));

	return(errNone);
}

Err DmReleaseRecord(DmOpenRef dbP, UInt16 index, Boolean dirty)
{
	if(index & ~dmMaxRecordIndex)
		return(dmErrIndexOutOfRange);

	dmdb_p db = dbP;

	dmdbrecord_p dbr = __DmGetRecord(db, index);

	__DmRecordDirty(dbr, &dirty);
	__DmRecordBusy(dbr, false);

	return(errNone);
}

Err DmReleaseResource(MemHandle resourceH)
{
	LOG("TODO"); return(-1);

	UNUSED(resourceH);
}

Err DmSetDatabaseInfo(const UInt16 cardNo, const LocalID dbID,
	const Char* nameP,
	UInt16 *const attributesP,
	UInt16 *const versionP,
	UInt32 *const crDateP,
	UInt32 *const modDateP,
	UInt32 *const bckUpDateP,
	UInt32 *const modNumP,
	LocalID *const appInfoIDP,
	LocalID *const sortInfoIDP,
	UInt32 *const typeP,
	UInt32 *const creatorP)
{
	dmdb_p db = __DmOpenDatabase(cardNo, dbID);

	PSET(appInfoIDP, db->appInfoID);
	PSET(attributesP, db->attributes);
	PSET(bckUpDateP, db->bckUpDate);
	PSET(crDateP, db->crDate);
	PSET(creatorP, db->creator);
	PSET(modDateP, db->modDate);
	PSET(modNumP, db->modNum);

	if(nameP) {
		const unsigned maxLen = sizeof(db->name) - 1;

		strncpy(db->name, nameP, maxLen);
		db->name[maxLen] = 0;
	}

	PSET(sortInfoIDP, db->sortInfoID);
	PSET(typeP, db->type);
	PSET(versionP, db->version);

	return(errNone); // TODO -- other errors may be returned
}

Err DmWrite(void *const recordP, const UInt32 offset, const void* srcP, const UInt32 bytes)
{
	dmdbrecord_p dbr = recordP;

	if(!dbr->h2db)
		return(-1); // TODO

	dmdb_p db = *dbr->h2db;
	if(!db)
		return(-1); // TODO

	// TODO -- validate record is part of database

	if(dbr->len < offset)
		return(-1); // TODO

	if(dbr->len < (offset + bytes))
		return(-1); // TODO

	memcpy(&dbr->data[offset], srcP, bytes);

	return(errNone);
}
