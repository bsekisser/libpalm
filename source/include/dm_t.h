#pragma once

/* **** */

#include "DataMgr.h"

/* **** */

#include "git/libbse/include/log.h"
#include "git/libbse/include/queue.h"
#include "git/libbse/include/unused.h"

/* **** */

typedef struct dmdb_t** dmdb_h;
typedef struct dmdb_t* dmdb_p;

typedef struct dmdbrecord_t*** dmdbrecord_p2h;
typedef struct dmdbrecord_t** dmdbrecord_h;
typedef struct dmdbrecord_t* dmdbrecord_p;

/* **** */

typedef struct dmdb_t {
	LocalID appInfoID;
	UInt16 attributes;
	UInt32 bckUpDate;
	UInt16 cardNo;
	UInt32 crDate;
	UInt32 creator;
	LocalID dbID;
	UInt32 modDate;
	UInt16 mode;
	UInt32 modNum;
	UInt16 openCount;
	dmdbrecord_p2h record[256];
	UInt8 recordBitmap[256];
	LocalID sortInfoID;
	UInt32 type;
	UInt16 version;
//
	char name[dmDBNameLength];
}dmdb_t;

typedef struct dmdbrecord_t {
	UInt16 attributes;
	dmdb_h h2db;
	dmdb_p p2db;
	size_t len;
	qelem_t nextRecordQElem;
//
	char data[];
}dmdbrecord_t;

/* **** */

Boolean __DmDatabaseAttr(dmdb_p const dbr, const UInt16 attribute, Boolean *const set);
dmdbrecord_p __DmNewRecord(dmdb_p db, UInt16 *const atP);
dmdb_p __DmOpenDatabase(UInt16 cardNo, LocalID dbID);
dmdbrecord_p __DmGetRecord(DmOpenRef const db, const UInt16 index);
void* __DmSetLastErr(Err);
Boolean __DmRecordAttr(dmdbrecord_p const dbr, const UInt16 attribute, Boolean *const set);
Boolean __DmSetDatabaseAttr(dmdb_p const dbr, const UInt16 attribute, const Boolean set);
Boolean __DmSetRecordAttr(dmdbrecord_p const dbr, const UInt16 attribute, const Boolean set);

#define __DmRecordBusy(_dbr, _set) __DmRecordAttr(_dbr, dmRecAttrBusy, _set)
#define __DmRecordDirty(_dbr, _set) __DmRecordAttr(_dbr, dmRecAttrDirty, _set)
#define __DmSetRecordBusy(_dbr, _set) __DmSetRecordAttr(_dbr, dmRecAttrBusy, _set)
#define __DmSetRecordDirty(_dbr, _set) __DmSetRecordAttr(_dbr, dmRecAttrDirty, _set)
