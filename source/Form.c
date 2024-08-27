#include "xForm.h"

/* **** */

#include "xEvent.h"

/* **** */

#include "git/libbse/include/log.h"
#include "git/libbse/include/unused.h"

/* **** */

#include <stdlib.h>
#include <string.h>

/* **** */

static UInt16 _event_form_id(EventPtr eventP)
{
	switch(eventP->eType) {
//		case frmCloseEvent:
//			return(eventP->data.frmClose.formID);
		case frmOpenEvent:
			return(eventP->data.frmOpen.formID);
		case frmGotoEvent:
			return(eventP->data.frmGoto.formID);
		default:
			break;
	}

	LOG_ACTION(exit(-1));
}

/* **** */

UInt16 FrmAlert(UInt16 alertID)
{
	LOG("alertID: %d (0x%04x)", alertID, alertID);

	LOG("TODO"); return(0);

//	UNUSED(alertID);
}

void FrmCloseAllForms(void)
{ LOG("TODO"); }

void FrmCopyLabel(FormType *const formP, const UInt16 labelID, const Char* newLabel)
{
	FormLabelType* labelP = (FormLabelType*)FrmGetObjectPtr(formP, labelID);

	char* dst = strncpy(labelP->text, newLabel, labelP->reserved);
	*dst = 0;

	if(formP->attr.usable) {
		if(labelP->attr.usable)
			LOG("TODO");
;//			FrmDrawLabel(formP, labelP);
	}
}

void FrmCopyTitle(FormType *const formP, const Char* newTitle)
{
//	char* dst = strncpy(formP->title, newTitle, form->reserved);
//	*dst = 0;

	if(formP->attr.usable) {
		if(formP->attr.visible)
			LOG("TODO");
;//			FrmDrawTitle(formP);
	}

	UNUSED(newTitle);
}

UInt16 FrmCustomAlert(UInt16 alertID,
	const char* s1, const char* s2, const char* s3)
{
	LOG("TODO"); return(0);

	UNUSED(alertID, s1, s2, s3);
}

void FrmDeleteForm(FormType* formP)
{
	LOG("TODO"); return;

	UNUSED(formP);
}

Boolean FrmDispatchEvent(EventPtr eventP)
{
	Boolean handled = false;

	LOG_ACTION(event_log_event(eventP));

	FormPtr formP = FrmGetFormPtr(_event_form_id(eventP)) ?: current_form;

	if(formP) {
		if(formP->handler) {
			LOG_ACTION(handled = formP->handler(eventP));
		} else
			LOG_ACTION(exit(-1));

		if(!handled)
			return(FrmHandleEvent(formP, eventP));
	}

	return(handled);
}

UInt16 FrmDoDialog(FormType* formP)
{
	LOG("TODO"); return(0);

	UNUSED(formP);
}

void FrmDrawForm(FormType* formP)
{
	LOG("TODO"); return;

	UNUSED(formP);
}

FormType* FrmGetActiveForm(void)
{
	WinPtr windowP = WinGetActiveWindow();

	return(windowP->windowFlags.dialog ? (FormPtr)windowP : 0);
}

UInt16 FrmGetActiveFormID(void)
{ return(FrmGetFormId(FrmGetActiveForm())); }

FormType* FrmGetFirstForm(void)
{
	WinPtr windowP = WinGetFirstWindow();

	while(windowP) {
		if(windowP->windowFlags.dialog)
			return((FormPtr)windowP);

		windowP = WinGetNextWindow(windowP);
	}

	return(0);
}

UInt16 FrmGetFormId(const FormType* formP)
{ return(formP ? formP->formId : 0); }

FormType* FrmGetFormPtr(const UInt16 formID)
{
	FormPtr formP = FrmGetFirstForm();

	while(formP) {
		if(formID == formP->formId)
			return(formP);

		formP = FrmGetNextForm(formP);
	}

	return(0);
}

FormPtr FrmGetNextForm(FormPtr formP)
{
	WinPtr windowP = ((WinPtr)formP) ?: WinGetFirstWindow();

	while(windowP) {
		if(windowP->windowFlags.dialog)
			return((FormPtr)windowP);

		windowP = windowP->nextWindow;
	}

	return(0);
}

UInt16 FrmGetNumberOfObjects(const FormType* formP)
{
	LOG("TODO"); return(0);

	UNUSED(formP);
}

UInt16 FrmGetObjectIndex(const FormType* formP, UInt16 objID)
{
	LOG("TODO"); return(0);

	UNUSED(formP, objID);
}

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

void* FrmGetObjectPtr(const FormType* formP, UInt16 objIndex)
{
	LOG("TODO"); return(0);

	UNUSED(formP, objIndex);
}

void FrmGotoForm(UInt16 formID)
{
	EventType event;

	if(current_form) {
		event.eType = frmCloseEvent;
		event.data.frmLoad.formID = current_form->formId;
		EvtAddEventToQueue(&event);
	}

	event.eType = frmLoadEvent;
	event.data.frmLoad.formID = formID;
	EvtAddEventToQueue(&event);

	event.eType = frmOpenEvent;
	event.data.frmOpen.formID = formID;
	EvtAddEventToQueue(&event);
}

Boolean FrmHandleEvent(FormPtr formP, EventPtr eventP)
{
	switch(eventP->eType) {
		case ctlEnterEvent:
		case ctlRepeatEvent:
		case ctlSelectEvent:
//			return(CtlHandleEvent(controlP, eventP));
		case fldEnterEvent:
//			return(FldHandleEvent(fieldP, eventP));
		case fldHeightChangedEvent:
//
		case frmCloseEvent:
		case frmGadgetEnterEvent:
		case frmGadgetMiscEvent:
		case frmTitleEnterEvent:
		case frmTitleSelectEvent:
			break;
		case frmUpdateEvent:
			FrmDrawForm(formP);
			return(1);
//
		case keyDownEvent:
//
		case lstEnterEvent:
//			return(LstHandleEvent(listP, eventP));
		case menuCmdBarOpenEvent:
//
		case menuEvent:
//			return(MenuHandleEvent(menuP, eventP));
		case penDownEvent:
//
		case popSelectEvent:
//
		case sclEnterEvent:
		case sclRepeatEvent:
//			return(SclHandleEvent(barP, eventP));
		case tblEnterEvent:
//			return(TblHandleEvent(tableP, eventP));
			break;
		default:
			return(0);
	}

	LOG_ACTION(event_log_event(eventP));
	return(0);

	UNUSED(formP);
}

void FrmHelp(UInt16 helpMsgID)
{
	LOG("TODO"); return;

	UNUSED(helpMsgID);
}

void FrmHideObject(FormType* formP, UInt16 objIndex)
{
	LOG("TODO"); return;

	UNUSED(formP, objIndex);
}

//FormType* FrmInitForm(UInt16 rscID)

void FrmSetActiveForm(FormType *const formP)
{ WinSetActiveWindow(&formP->window); }

void FrmSetEventHandler(FormType* formP, FormEventHandlerType* handler)
{
	if(!formP) return;

	formP->handler = handler;
}

void FrmSetFocus(FormType* formP, UInt16 fieldIndex)
{
	formP->focus = fieldIndex;

	LOG("TODO");
}

void FrmSetMenu(FormType *const formP, const UInt16 menuRscID)
{
	LOG("TODO"); return;

	UNUSED(formP, menuRscID);
}

void FrmSetTitle(FormType *const formP, Char *const newTitle)
{
//	formP->title = newTitle;

	if(formP->attr.usable) {
		if(formP->attr.visible)
			LOG("TODO");
;//			FrmDrawLabel(formP, labelP);
	}

	UNUSED(newTitle);
}

void FrmShowObject(FormType* formP, UInt16 objIndex)
{
	LOG("TODO"); return;

	UNUSED(formP, objIndex);
}
