#include "config.h"

#include "xForm.h"
#include "xWindow.h"

/* **** */

#include "xEvent.h"

/* **** */

#include "git/libbse/include/log.h"
#include "git/libbse/include/unused.h"

/* **** */

#include <assert.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

/* **** */

struct config_form_t {
	struct {
		struct {
			unsigned entry:1;
			unsigned exit:1;
		}at;
	}trace;
}form_config;

/* **** */

__attribute__((constructor))
static void __form_manager_confg_init(void)
{
	AT_INIT(LOG());

	(void)memset(&form_config, 0, sizeof(form_config));

//	form_config.trace.at.entry = 1;
//	form_config.trace.at.exit = 1;
}

static int _event_form_id(EventPtr eventP)
{
	PEDANTIC(assert(eventP));

	switch(eventP->eType) {
		case frmCloseEvent:
			return(eventP->data.frmClose.formID);
		case frmGotoEvent:
			return(eventP->data.frmGoto.formID);
		case frmLoadEvent:
			return(eventP->data.frmLoad.formID);
		case frmOpenEvent:
			return(eventP->data.frmOpen.formID);
		case frmTitleEnterEvent:
			return(eventP->data.frmTitleEnter.formID);
		case frmTitleSelectEvent:
			return(eventP->data.frmTitleSelect.formID);
		case frmUpdateEvent:
			return(eventP->data.frmUpdate.formID);
		default:
			break;
	}

	return(-1);
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
	PEDANTIC(assert(formP));
	PEDANTIC(assert(newLabel));

	FormLabelType* labelP = (FormLabelType*)FrmGetObjectPtr(formP, labelID);

	PEDANTIC(assert(labelP));

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
	PEDANTIC(assert(formP));

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
	PEDANTIC(assert(formP));

	for(unsigned x = 0; x < formP->numObjects; x++) {
		FormObjListTypePtr formObject = &formP->objects[x];
		if(!formObject) continue;

		FormObjectKind objectType = formObject->objectType;

		switch(objectType) {
			default:
				LOG_START("TODO: delete formObject: 0x%016" PRIxPTR, (uintptr_t)formObject);
				LOG_END(", objectKind: 0x%08x", objectType);
				break;
		}
	}

	MemPtrFree(formP->objects);
	formP->objects = 0;

	if(formP->helpRscId)
		LOG("TODO: delete form help");

	if(formP->menuRscId)
		LOG("TODO: delete form menu");

	WinDeleteWindow(&formP->window, formP->attr.saveBehind);
}

Boolean FrmDispatchEvent(EventPtr eventP)
{
	PEDANTIC(assert(eventP));

	Boolean handled = false;

	LOG_ACTION(event_log_event(eventP));

	UInt16 formID = _event_form_id(eventP);
	FormPtr formP = (0 < (Int16)formID) ? FrmGetFormPtr(formID) : current_form;

	if(formP) {
		if(formP->handler) {
			handled = formP->handler(eventP);
		} else
			LOG_ACTION(exit(-1));

		if(!handled)
			return(FrmHandleEvent(formP, eventP));
	}

	return(handled);
}

UInt16 FrmDoDialog(FormType* formP)
{
	PEDANTIC(assert(formP));

	LOG("TODO"); return(0);

	UNUSED(formP);
}

void FrmDrawForm(FormType* formP)
{
	PEDANTIC(assert(formP));

	WinPtr form_window = &formP->window;
	WinPtr saved_draw_window = WinSetDrawWindow(form_window);

	if(0) if(saved_draw_window && (saved_draw_window != form_window)) {
		if(formP->attr.saveBehind)
;//			_FrmEraseForm(formP, true);
	}

	WinDrawWindow(form_window);

	LOG("TODO");

	WinSetDrawWindow(saved_draw_window);
}

FormType* FrmGetActiveForm(void)
{
	WinPtr windowP = WinGetActiveWindow();

	if(!windowP) return(0);

	return(windowP->windowFlags.dialog ? (FormPtr)windowP : 0);
}

UInt16 FrmGetActiveFormID(void)
{ return(FrmGetFormId(FrmGetActiveForm())); }

FormType* FrmGetFirstForm(void)
{
	if(form_config.trace.at.entry) LOG();

	WinPtr windowP = WinGetFirstWindow();

	while(windowP) {
		if(windowP->windowFlags.dialog)
			return((FormPtr)windowP);

		windowP = WinGetNextWindow(windowP);
	}

	if(form_config.trace.at.exit) LOG();

	return(0);
}

UInt16 FrmGetFormId(const FormType* formP)
{ return(formP ? formP->formId : 0); }

FormType* FrmGetFormPtr(const UInt16 formID)
{
	if(form_config.trace.at.entry) {
		LOG_START(">>");
		LOG_END(" formID: 0x%04x (%u)", formID, formID);
	}

	FormPtr formP = FrmGetFirstForm();

	while(formP) {
		if(formID == formP->formId)
			return(formP);

		formP = FrmGetNextForm(formP);
	}

	if(form_config.trace.at.exit) LOG("<<");

	return(0);
}

FormPtr FrmGetNextForm(FormPtr formP)
{
	WinPtr windowP = ((WinPtr)formP) ? &formP->window : WinGetFirstWindow();

	while(windowP) {
		if(windowP->windowFlags.dialog)
			return((FormPtr)windowP);

		windowP = windowP->nextWindow;
	}

	return(0);
}

UInt16 FrmGetNumberOfObjects(const FormType* formP)
{
	PEDANTIC(assert(formP));

	LOG("TODO"); return(0);

	UNUSED(formP);
}

UInt16 FrmGetObjectIndex(const FormType* formP, UInt16 objID)
{
	PEDANTIC(assert(formP));

	LOG("TODO"); return(0);

	UNUSED(formP, objID);
}

UInt16 FrmGetObjectIndexFromPtr(const FormType* formP, void *const objP)
{
	PEDANTIC(assert(formP));

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
	PEDANTIC(assert(formP));

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
	PEDANTIC(assert(formP));
	PEDANTIC(assert(eventP));

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
			FrmDeleteForm(formP);
			return(1);
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
	PEDANTIC(assert(formP));

	LOG("TODO"); return;

	UNUSED(formP, objIndex);
}

//FormType* FrmInitForm(UInt16 rscID)

void FrmSetActiveForm(FormType *const formP)
{ WinSetActiveWindow(&formP->window); }

void FrmSetEventHandler(FormType* formP, FormEventHandlerType* handler)
{
	PEDANTIC(assert(formP));

	if(!formP) return;

	formP->handler = handler;
}

void FrmSetFocus(FormType* formP, UInt16 fieldIndex)
{
	PEDANTIC(assert(formP));

	formP->focus = fieldIndex;

	LOG("TODO");
}

void FrmSetMenu(FormType *const formP, const UInt16 menuRscID)
{
	PEDANTIC(assert(formP));

	LOG("TODO"); return;

	UNUSED(formP, menuRscID);
}

void FrmSetTitle(FormType *const formP, Char *const newTitle)
{
	PEDANTIC(assert(formP));

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
	PEDANTIC(assert(formP));

	LOG("TODO"); return;

	UNUSED(formP, objIndex);
}
