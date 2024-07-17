#pragma once

/* **** */

#include "git/libbse/include/log.h"
#include "git/libbse/include/unused.h"

/* **** */

typedef struct dmdb_t* dmdb_p;
typedef struct dmdb_t {
	LocalID appInfoID;
	UInt16 attributes;
	UInt32 bckUpDate;
	UInt32 crDate;
	UInt32 creator;
	UInt32 modDate;
	UInt32 modNum;
	LocalID sortInfoID;
	UInt32 type;
	UInt16 version;
//
	char name[32];
}dmdb_t;

dmdb_p dmdbhdr_for_card_local(UInt16 cardNo, LocalID dbID)
{
	LOG("TODO"); return(0);
	UNUSED(cardNo, dbID);
}
