//
// database part in company date, except for filling out the list, think
// how to separate it kosherly
//
// part of the code will be reused in the goods tab
//


#include "globals.h"
#include "tabfirma.h"
#include "fakdata.h"
#include "dialnalodb.h"

#include <Alert.h>
#include <Box.h>
#include <Button.h>
#include <CheckBox.h>
#include <TabView.h>
#include <TextControl.h>
#include "santa/ColumnListView.h"
#include <stdio.h>

const uint32 LIST_INV	= 'TFLI';
const uint32 LIST_SEL	= 'TFLS';
const uint32 BUT_NEW	= 'TFBN';
const uint32 BUT_DEL	= 'TFBD';
const uint32 BUT_RESTORE= 'TFBR';
const uint32 BUT_SAVE	= 'TFBS';
const uint32 BUT_PAYMENT= 'TFBP';
const uint32 DC			= 'TFDC';

tabFirma::tabFirma(BTabView *tv, sqlite *db, BHandler *hr) : beFakTab(tv, db, hr) {

	curdata = new firmadat(db);
	this->dirty = false;

	this->tab->SetLabel("Kontrahenci [F4]");
	BRect r;
	r = this->view->Bounds();

	// columnlistview
	r.left = 5; r.right = 160; r.top = 30; r.bottom = 490;
	CLVContainerView *containerView;
	list = new ColumnListView(r, &containerView, NULL, B_FOLLOW_LEFT|B_FOLLOW_TOP_BOTTOM,
		B_WILL_DRAW|B_FRAME_EVENTS|B_NAVIGABLE, B_SINGLE_SELECTION_LIST, false, true, true, true,
		B_FANCY_BORDER);
	list->AddColumn(new CLVColumn("Symbol", 54, CLV_TELL_ITEMS_WIDTH|CLV_HEADER_TRUNCATE|CLV_SORT_KEYABLE));
	list->AddColumn(new CLVColumn("Nazwa", 100, CLV_TELL_ITEMS_WIDTH|CLV_HEADER_TRUNCATE|CLV_SORT_KEYABLE));
	list->SetSortFunction(CLVEasyItem::CompareItems);
	this->view->AddChild(containerView);
	list->SetInvocationMessage(new BMessage(LIST_INV));
	list->SetSelectionMessage(new BMessage(LIST_SEL));
	// buttons
	but_new = new BButton(BRect(30,0,140,24), "tf_but_new", "New contractor [F5]", new BMessage(BUT_NEW), B_FOLLOW_LEFT|B_FOLLOW_TOP);
	but_del = new BButton(BRect(30,510,140,534), "tf_but_del", "Delete selected [F8]", new BMessage(BUT_DEL), B_FOLLOW_LEFT|B_FOLLOW_BOTTOM);
	but_restore = new BButton(BRect(235,510,325,534), "tf_but_restore", "Restore [F6]", new BMessage(BUT_RESTORE), B_FOLLOW_LEFT|B_FOLLOW_BOTTOM);
	but_save = new BButton(BRect(580,510,670,534), "tf_but_save", "Save", new BMessage(BUT_SAVE), B_FOLLOW_RIGHT|B_FOLLOW_BOTTOM);
	but_payments = new BButton(BRect(405,510,485,534), "tf_but_payment", "Receivables [F9]", new BMessage(BUT_PAYMENT), B_FOLLOW_LEFT_RIGHT|B_FOLLOW_BOTTOM);
	this->view->AddChild(but_new);
	this->view->AddChild(but_del);
	this->view->AddChild(but_restore);
	this->view->AddChild(but_save);
	this->view->AddChild(but_payments);
	but_new->ResizeToPreferred();
	but_del->ResizeToPreferred();
	but_restore->ResizeToPreferred();
	but_save->ResizeToPreferred();
	but_payments->ResizeToPreferred();
	// box1
	box1 = new BBox(BRect(230,30,670,180), "tf_box1");
	box1->SetLabel("Dane adresowe");
	this->view->AddChild(box1);
	// box1-stuff
	data[0] = new BTextControl(BRect(10,15,270,35), "tfd0", "Name", NULL, new BMessage(DC));
	data[1] = new BTextControl(BRect(280,15,420,35), "tfd1", "Symbol", NULL, new BMessage(DC));
	data[2] = new BTextControl(BRect(10,50,420,65), "tfd2", "Address", NULL, new BMessage(DC));
	data[3] = new BTextControl(BRect(10,80,150,95), "tfd3", "Code", NULL, new BMessage(DC));
	data[4] = new BTextControl(BRect(160,80,420,95), "tfd4", "City", NULL, new BMessage(DC));
	data[5] = new BTextControl(BRect(10,110,200,125), "tfd5", "Tel.", NULL, new BMessage(DC));
	data[6] = new BTextControl(BRect(210,110,420,125), "tfd6", "Email", NULL, new BMessage(DC));
	box1->AddChild(data[0]); box1->AddChild(data[1]);
	box1->AddChild(data[2]);
	box1->AddChild(data[3]); box1->AddChild(data[4]);
	box1->AddChild(data[5]); box1->AddChild(data[6]);
	// box2
	box2 = new BBox(BRect(230,190,670,340), "tf_box2");
	box2->SetLabel("Company Details");
	this->view->AddChild(box2);
	// box2-stuff
	r.left = 10; r.top = 20; r.right = 420; r.bottom = 35;
	data[7] = new BTextControl(r, "tfd7", "NIP", NULL, new BMessage(DC)); r.OffsetBy(0, 30);
	data[8] = new BTextControl(r, "tfd8", "REGON", NULL, new BMessage(DC)); r.OffsetBy(0, 30);
	data[9] = new BTextControl(r, "tfd9", "Bank", NULL, new BMessage(DC)); r.OffsetBy(0, 30);
	data[10] = new BTextControl(r, "tfd10", "Account No", NULL, new BMessage(DC));
	box2->AddChild(data[7]); box2->AddChild(data[8]);
	box2->AddChild(data[9]); box2->AddChild(data[10]);
	// box3
	box3 = new BBox(BRect(230,360,380,440), "tf_box3");
	this->view->AddChild(box3);
	// box3-stuff
	recipient = new BCheckBox(BRect(10,15,100,35), "tf_co", "Recipient", new BMessage(DC));
	supplier = new BCheckBox(BRect(10,50,100,65), "tf_cd", "Supplier", new BMessage(DC));
	box3->AddChild(recipient); box3->AddChild(provider);
	// box4
	box4 = new BBox(BRect(520,360,670,440), "tf_box4");
	this->view->AddChild(box4);
	// box4-stuff
	active = new BCheckBox(BRect(10,15,100,35), "tfca", "Active", new BMessage(DC));
	blocked = new BCheckBox(BRect(10,50,100,65), "tf_cz", "Blocked", new BMessage(DC));
	box4->AddChild(active); box4->AddChild(blocked);
	// fix widths
	int i;
	// first set them to be enough
	for (i=0;i<=10;i++) {
		data[i]->SetDivider(be_plain_font->StringWidth(data[i]->Label())+5);
	}
	// align in columns
	float d;
	d = max(data[0]->Divider(), data[2]->Divider());
	d = max(data[3]->Divider(), d);
	d = max(data[5]->Divider(), d);
	data[0]->SetDivider(d); data[2]->SetDivider(d);
	data[3]->SetDivider(d); data[5]->SetDivider(d);
	d = max(data[7]->Divider(), data[8]->Divider());
	d = max(data[9]->Divider(), d);
	d = max(data[10]->Divider(), d);
	data[7]->SetDivider(d); data[8]->SetDivider(d);
	data[9]->SetDivider(d); data[10]->SetDivider(d);
	//
	but_save->MakeDefault(true);
	//
	curdataToTab();
	RefreshIndexList();
}

tabFirma::~tabFirma() {
	delete curdata;
}

void tabFirma::curdataFromTab(void) {
	int i;
	for (i=0;i<=10;i++) {
		curdata->data[i] = data[i]->Text();
	}
	curdata->recipient = (recipient->Value() == B_CONTROL_ON);
	curdata->supplier = (supplier->Value() == B_CONTROL_ON);
	curdata->active = (active->Value() == B_CONTROL_ON);
	curdata->blocked = (blocked->Value() == B_CONTROL_ON);
}

void tabFirma::curdataToTab(void) {
	int i;
	for (i=0;i<=10;i++) {
		data[i]->SetText(curdata->data[i].String());
	}
	recipient->SetValue(curdata->recipient ? B_CONTROL_ON : B_CONTROL_OFF);
	supplier->SetValue(curdata->supplier ? B_CONTROL_ON : B_CONTROL_OFF);
	active->SetValue(curdata->active ? B_CONTROL_ON : B_CONTROL_OFF);
	blocked->SetValue(curdata->blocked ? B_CONTROL_ON : B_CONTROL_OFF);
	updateTab();
}

void tabFirma::updateTab(void) {
	bool state = (zablokowany->Value() != B_CONTROL_ON);
	BMessage *msg = new BMessage(MSG_NAMECHANGE);
	msg->AddString("_newtitle", data[0]->Text());
	handler->Looper()->PostMessage(msg);

	int i;
	for (i=0;i<=10;i++) {
		data[i]->SetEnabled(state);
	}
	recipient->SetEnabled(state);
	provider->SetEnabled(state);
	active->SetEnabled(state);
}

// perform checks against supplied data
bool tabFirma::validateTab(void) {
	BAlert *error;
	BString sql, tmp;
	int i;
	// name - not empty
	if (strlen(data[0]->Text()) == 0) {
		error = new BAlert(APP_NAME, "The contractor's name was not entered!", "OK", NULL, NULL, B_WIDTH_AS_USUAL, B_WARNING_ALERT);
		error->Go();
		data[0]->MakeFocus();
		return false;
	}
	// name - unique
	tmp = data[0]->Text(); tmp.ReplaceAll("'","''");	// sql quote
	sql = "SELECT id FROM firma WHERE name = '"; sql += tmp; sql += "'";
	i = toint(execSQL(sql.String()));
	if (((curdata->id < 0) && ( i!= 0 )) || ((curdata->id > 0) && (i != 0) && (i != curdata->id))) {
		error = new BAlert(APP_NAME, "The company name is not unique!", "OK", NULL, NULL, B_WIDTH_AS_USUAL, B_WARNING_ALERT);
		error->Go();
		data[0]->MakeFocus();
		return false;
	}
	// symbol - not empty
	if (strlen(data[1]->Text()) == 0) {
		error = new BAlert(APP_NAME, "The contractor's symbol has not been entered", "OK", NULL, NULL, B_WIDTH_AS_USUAL, B_WARNING_ALERT);
		error->Go();
		data[1]->MakeFocus();
		return false;
	}
	// symbol - unique
	tmp = data[1]->Text(); tmp.ReplaceAll("'","''");	// sql quote
	sql = "SELECT id FROM firma WHERE symbol = '"; sql += tmp; sql += "'";
	i = toint(execSQL(sql.String()));
	if (((curdata->id < 0) && ( i!= 0 )) || ((curdata->id > 0) && (i != 0) && (i != curdata->id))) {
		error = new BAlert(APP_NAME, "The contractor's symbol is not unique!", "OK", NULL, NULL, B_WIDTH_AS_USUAL, B_WARNING_ALERT);
		error->Go();
		data[1]->MakeFocus();
		return false;
	}
	// address - all data
	if ((strlen(data[2]->Text())==0) || (strlen(data[3]->Text())==0) || (strlen(data[4]->Text())==0)) {
		error = new BAlert(APP_NAME, "The contractor's address is not complete.\Continue?", "Yes", "No", NULL, B_WIDTH_AS_USUAL, B_WARNING_ALERT);
		if (error->Go() == 1) {
			data[2]->MakeFocus();
			return false;
		}
	}
	// NIP - not empty, correct
	if (strlen(data[7]->Text())==0) {
		error = new BAlert(APP_NAME, "The contractor's Tax Identification Number has not been entered.\nContinue?",  "Yes", "No", NULL, B_WIDTH_AS_USUAL, B_WARNING_ALERT);
		if (error->Go() == 1) {
			data[7]->MakeFocus();
			return false;
		}
	}
	// REGON - not empty, correct
	if (strlen(data[8]->Text())==0) {
		error = new BAlert(APP_NAME, "The contractor's REGON number has not been entered.\nContinue?",  "Yes", "No", NULL, B_WIDTH_AS_USUAL, B_WARNING_ALERT);
		if (error->Go() == 1) {
			data[8]->MakeFocus();
			return false;
		}
	}
	// account number - not empty, correct
	if (strlen(data[10]->Text())==0) {
		error = new BAlert(APP_NAME, "The contractor's account number has not been entered.\nContinue?",  "Yes", "No", NULL, B_WIDTH_AS_USUAL, B_WARNING_ALERT);
		if (error->Go() == 1) {
			data[10]->MakeFocus();
			return false;
		}
	}
	return true;
}

void tabFirma::MessageReceived(BMessage *Message) {
	switch (Message->what) {
		case DC:
			this->dirty = true;
			updateTab();
			break;
		case B_F5_KEY:
		case BUT_NEW:
			if (CommitCurdata()) {
				list->DeselectAll();
				// clear curdata
				curdata->clear();
				// refresh tabs
				curdataToTab();
				data[0]->MakeFocus();
			}
			break;
		case B_F6_KEY:
		case BUT_RESTORE:
			DoFetchCurdata();
			break;
		case B_F8_KEY:
		case BUT_DEL:
			DoDeleteCurdata();
			break;
		case BUT_SAVE:
			curdataFromTab();
			DoCommitCurdata();
			curdataToTab();
			break;
		case B_F9_KEY:
		case BUT_PAYMENT:
			{	dialNalodb *nalodbDialog;
				if (curdata->id>0) {
					nalodbDialog = new dialNalodb(dbData, curdata->data[0].String());
				}
			}
		case MSG_REQFIRMALIST:
			{
				RefreshIndexList();
				BMessage *msg = new BMessage(MSG_REQFIRMAUP);
				handler->Looper()->PostMessage(msg);
				break;
			}
		case LIST_SEL:
		case LIST_INV:
			{	int i = list->CurrentSelection(0);
				if (i>=0) {
					i = ((tab2ListItem*)list->ItemAt(list->CurrentSelection(0)))->Id();
					if (i>=0)
						ChangedSelection(i);
				} else {
					// deselection, what to do?
				}
				break;
			}
		default:
			break;
	}
}

void tabFirma::ChangedSelection(int newid) {
	if (!(CommitCurdata())) {
		// XXX do nothing if cancel, restore old selection?
		return;
	}
	// fetch and store into new data
	curdata->id = newid;
	DoFetchCurdata();
}

void tabFirma::DoCommitCurdata(void) {
	if (!(validateTab()))
		return;
	curdata->commit();
	this->dirty = false;
	BMessage *msg = new BMessage(MSG_REQFIRMAUP);
	handler->Looper()->PostMessage(msg);
	RefreshIndexList();
}

void tabFirma::DoDeleteCurdata(void) {
// XXX ask for confimation?
	curdata->del();
	curdataToTab();
	BMessage *msg = new BMessage(MSG_REQFIRMAUP);
	handler->Looper()->PostMessage(msg);
	RefreshIndexList();
}

void tabFirma::DoFetchCurdata(void) {
	if (curdata->id >=0) {
		curdata->fetch();
		this->dirty = false;
		curdataToTab();
	}
}

void tabFirma::RefreshIndexList(void) {
	// clear current list
	if (list->CountItems()>0) {
		tab2ListItem *anItem;
		for (int i=0; (anItem=(tab2ListItem*)list->ItemAt(i)); i++)
			delete anItem;
		if (!list->IsEmpty())
			list->MakeEmpty();
	}
	// select list from db
	int nRows, nCols;
	char **result;
	sqlite_get_table(dbData, "SELECT id, symbol, name FROM firma ORDER BY id", &result, &nRows, &nCols, &dbErrMsg);
	if (nRows < 1) {
		// no entries
	} else {
		for (int i=1;i<=nRows;i++)
			list->AddItem(new tab2ListItem(toint(result[i*nCols+0]), result[i*nCols+1], result[i*nCols+2]));
	}
	sqlite_free_table(result);
}
