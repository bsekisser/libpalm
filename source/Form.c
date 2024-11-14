#include "config.h"

#define ALLOW_ACCESS_TO_INTERNALS_OF_FORMS
#define ALLOW_ACCESS_TO_INTERNALS_OF_WINDOWS

#include "xEvent.h"
#include "xForm.h"
#include "xWindow.h"
#include "xUIResources.h"

/* **** */

#include "git/libbse/include/err_test.h"
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

#define TRACE form_config.trace

/* **** */

__attribute__((constructor))
static void __form_manager_confg_init(void)
{
	AT_INIT(LOG());

	(void)memset(&form_config, 0, sizeof(form_config));

	TRACE.at.entry = 1;
	TRACE.at.exit = 1;
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
	LOG("-- TODO -- alertID: %d (0x%04x)", alertID, alertID);
	return(0);
}

void FrmCloseAllForms(void)
{
	LOG("TODO");
}

void FrmCopyLabel(FormRef formP, const UInt16 labelID, const Char* newLabel)
{
	TRACE_ENTRY();

	PEDANTIC(assert(formP));
	PEDANTIC(assert(newLabel));

	FormLabelRef labelP = (FormLabelPtr)FrmGetObjectPtr(formP, labelID);

	PEDANTIC(assert(labelP));

	char* dst = strncpy(labelP->text, newLabel, labelP->reserved);
	*dst = 0;

	if(formP->attr.usable) {
		if(labelP->attr.usable)
			LOG("TODO");
;//			FrmDrawLabel(formP, labelP);
	}

	TRACE_EXIT();
}

void FrmCopyTitle(FormRef formP, const Char* newTitle)
{
	TRACE_ENTRY();

	PEDANTIC(assert(formP));

//	char* dst = strncpy(formP->title, newTitle, form->reserved);
//	*dst = 0;

	if(formP->attr.usable) {
		if(formP->attr.visible)
			LOG("TODO");
;//			FrmDrawTitle(formP);
	}

	TRACE_EXIT();

	UNUSED(newTitle);
}

UInt16 FrmCustomAlert(UInt16 alertID,
	const char* s1, const char* s2, const char* s3)
{
	LOG("TODO");

	return(0);

	UNUSED(alertID, s1, s2, s3);
}

void FrmDeleteForm(FormRef formP)
{
	TRACE_ENTRY();

	PEDANTIC(assert(formP));

	for(unsigned x = 0; x < formP->numObjects; x++) {
		FormObjListRef formObject = &formP->objects[x];
		FormObjectRef object = &formObject->object;

		if(!object->ptr) continue;

		FormObjectKind objectType = formObject->objectType;

		switch(objectType) {
			case frmLabelObj:
				FrmDeleteLabel(formObject->object.label);
				break;
			default:
				LOG_START("TODO: delete formObject: 0x%016" PRIxPTR, (uintptr_t)formObject);
				LOG_END(", objectKind: 0x%08x", objectType);
				break;
		}

		object->ptr = 0;
	}

	MemPtrFree(formP->objects);
	formP->objects = 0;

	if(formP->helpRscId)
		LOG("TODO: delete form help");

	if(formP->menuRscId)
		LOG("TODO: delete form menu");

	WinDeleteWindow(&formP->window, formP->attr.saveBehind);

	TRACE_EXIT();
}

void FrmDeleteLabel(FormLabelRef label)
{
	MemPtrFree(label->text);
	MemPtrFree(label);
}

Boolean FrmDispatchEvent(EventPtr eventP)
{
	PEDANTIC(TRACE_ENTRY());

	PEDANTIC(assert(eventP));

	Boolean handled = false;

	LOG_ACTION(event_log_event(eventP));

	UInt16 formID = _event_form_id(eventP);
	FormRef formP = (0 < (Int16)formID) ? FrmGetFormPtr(formID) : current_form;

	if(formP) {
		if(formP->handler) {
			handled = formP->handler(eventP);
		} else
			LOG_ACTION(exit(-1));

		if(!handled)
			return(FrmHandleEvent(formP, eventP));
	}

	PEDANTIC(TRACE_EXIT());

	return(handled);
}

UInt16 FrmDoDialog(FormRef formP)
{
	TRACE_ENTRY();

	PEDANTIC(assert(formP));

	TRACE_EXIT();
	return(0);

	UNUSED(formP);
}

void FrmDrawForm(FormRef formP)
{
	if(TRACE_ENTRY_START("TODO")) {
		LOG_END(" formP: 0x%016" PRIxPTR, (uintptr_t)formP);
	}

	PEDANTIC(assert(formP));

	WinPtr const form_window = &formP->window;
	WinPtr const saved_draw_window = WinSetDrawWindow(form_window);

	if(0) if(saved_draw_window && (saved_draw_window != form_window)) {
		if(formP->attr.saveBehind) {
;//			_FrmEraseForm(formP, true);
		}
	}

	WinDrawWindow(form_window);

	for(unsigned i = 0; i < formP->numObjects; i++)
	{
		FormObjListRef formObject = &formP->objects[i];
		FormObjectRef object = &formObject->object;

		if(!object->ptr) continue;

		const FormObjectKind objectType = formObject->objectType;

		switch(objectType) {
			case frmLabelObj:
				FrmDrawLabel(object->label);
				break;
			default:
				LOG("unhandled object type %u", objectType);
				break;
		}
	}

	WinSetDrawWindow(saved_draw_window);

	TRACE_EXIT();
}

void FrmDrawLabel(FormLabelRef label)
{
	const int len = strlen(label->text);

	WinDrawChars(label->text, len, label->pos.x, label->pos.y);
}

FormPtr FrmGetActiveForm(void)
{
	TRACE_ENTRY();

	WinPtr const windowP = WinGetActiveWindow();
	const unsigned is_dialog = windowP ? windowP->windowFlags.dialog : 0;

	FormRef formP = is_dialog ? (FormPtr)windowP : 0;

	if(TRACE_EXIT_START()) {
		_LOG_("windowP: 0x%016" PRIxPTR, (uintptr_t)windowP);
		_LOG_(".dialog: %01u", is_dialog);
		LOG_END(", formP: 0x%016" PRIxPTR, (uintptr_t)formP);
	}

	return(formP);
}

UInt16 FrmGetActiveFormID(void)
{
	TRACE_ENTRY();

	UInt16 formID = FrmGetFormId(FrmGetActiveForm());

	TRACE_EXIT("formID: 0x%04x", formID);

	return(formID);
}

FormPtr FrmGetFirstForm(void)
{
	TRACE_ENTRY();

	WinPtr windowP = WinGetFirstWindow();

	while(windowP) {
		if(windowP->windowFlags.dialog)
			break;

		windowP = WinGetNextWindow(windowP);
	}

	TRACE_EXIT("formP: 0x%016" PRIxPTR, (uintptr_t)windowP);

	return((FormPtr)windowP);
}

UInt16 FrmGetFormId(const FormType *const formP)
{
	TRACE_ENTRY();

	UInt16 formId = formP ? formP->formId : 0;

	TRACE_EXIT();

	return(formId);
}

FormPtr FrmGetFormPtr(const UInt16 formID)
{
	TRACE_ENTRY("formID: 0x%04x (%u)", formID, formID);

	FormPtr formP = FrmGetFirstForm();

	while(formP) {
		if(formID == formP->formId)
			return(formP);

		formP = FrmGetNextForm(formP);
	}

	TRACE_EXIT();

	return(0);
}

FormPtr FrmGetNextForm(FormRef formP)
{
	TRACE_ENTRY();

	WinPtr windowP = ((WinPtr)formP) ? &formP->window : WinGetFirstWindow();

	while(windowP) {
		if(windowP->windowFlags.dialog)
			break;

		windowP = windowP->nextWindow;
	}

	TRACE_EXIT();

	return((FormPtr)windowP);
}

UInt16 FrmGetNumberOfObjects(const FormType *const formP)
{
	PEDANTIC(assert(formP));

	LOG("TODO"); return(0);

	UNUSED(formP);
}

UInt16 FrmGetObjectIndex(const FormType *const formP, UInt16 objID)
{
	PEDANTIC(assert(formP));

	LOG("TODO"); return(0);

	UNUSED(formP, objID);
}

UInt16 FrmGetObjectIndexFromPtr(const FormType *const formP, void *const objP)
{
	if(!TRACE_ENTRY("TODO"))
		LOG("TODO");

	PEDANTIC(assert(formP));

//	UInt16 objIndex = frmInvalidObjectId;
	UInt16 numObjexts = FrmGetNumberOfObjects(formP);
	for(UInt16 index = 0; index < numObjexts; index++) {
		if(FrmGetObjectPtr(formP, index) == objP)
			return(index);
	}

	TRACE_EXIT();

	return(frmInvalidObjectId);
}

void* FrmGetObjectPtr(const FormType *const formP, UInt16 objIndex)
{
	PEDANTIC(assert(formP));

	LOG("TODO"); return(0);

	UNUSED(formP, objIndex);
}

void FrmGotoForm(UInt16 formID)
{
	TRACE_ENTRY("formID: 0x%02x", formID);

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

	TRACE_EXIT();
}

Boolean FrmHandleEvent(FormRef formP, EventPtr eventP)
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

void FrmHideObject(FormRef formP, UInt16 objIndex)
{
	if(!TRACE_ENTRY("TODO"))
		LOG("TODO");

	PEDANTIC(assert(formP));

	TRACE_EXIT();

	UNUSED(formP, objIndex);
}

FormPtr FrmInitForm(UInt16 rscID)
{
	FormRef formP = ResLoadForm(rscID);
	ERR_NULL(formP);

	WinPtr const fw = &formP->window;

	/* **** */

	formP->attr.usable = 1;
	formP->handler = 0;
	formP->focus = 0;
	fw->windowFlags.dialog = 1;

	/* **** */

	_WinCreateWindow(fw, &fw->windowBounds, simpleFrame, fw->windowFlags.modal, true);

	return(formP);
}

void FrmSetActiveForm(FormRef formP)
{
	TRACE_ENTRY("formP: 0x%016" PRIxPTR, (uintptr_t)formP);

	WinSetActiveWindow(&formP->window);

	TRACE_EXIT();
}

void FrmSetEventHandler(FormRef formP, FormEventHandlerRef handler)
{
	TRACE_ENTRY();

	PEDANTIC(assert(formP));

	if(formP) {
		formP->handler = handler;
	}

	TRACE_EXIT();
}

void FrmSetFocus(FormRef formP, UInt16 fieldIndex)
{
	if(!TRACE_ENTRY("TODO"))
		LOG("TODO");

	PEDANTIC(assert(formP));

	formP->focus = fieldIndex;

	TRACE_EXIT();
}

void FrmSetMenu(FormRef formP, const UInt16 menuRscID)
{
	if(!TRACE_ENTRY("TODO"))
		LOG("TODO");

	PEDANTIC(assert(formP));

	TRACE_EXIT();

	UNUSED(formP, menuRscID);
}

void FrmSetTitle(FormRef formP, Char *const newTitle)
{
	TRACE_ENTRY();

	PEDANTIC(assert(formP));

//	formP->title = newTitle;

	if(formP->attr.usable) {
		if(formP->attr.visible)
			LOG("TODO");
;//			FrmDrawLabel(formP, labelP);
	}

	TRACE_EXIT();

	UNUSED(newTitle);
}

void FrmShowObject(FormRef formP, UInt16 objIndex)
{
	if(!TRACE_ENTRY("TODO"))
		LOG("TODO");

	PEDANTIC(assert(formP));

	TRACE_EXIT();

	UNUSED(formP, objIndex);
}
