#include "Form.h"

/* **** */

#include "libbse/include/log.h"
//#include "libbse/include/unused.h"

/* **** */

UInt16 FrmGetObjectIndexFromPtr(const FormType* formP, void *const objP)
{
	LOG("TODO");

//	UInt16 objIndex = frmInvalidObjectId;
	UInt16 numObjexts = FrmGetNumberOfObjects(formP);
	for(UInt16 index = 0; index < numObjexts; index++) {
		if(FrmGetObjectPtr(formP, index) == objP)
			return(index);
	}

	return(frmInvalidObjectId);
}
