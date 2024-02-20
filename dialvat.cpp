
#include <Alert.h>
#include <Box.h>
#include <Button.h>
#include <String.h>
#include <TextControl.h>
#include <View.h>
#include "santa/ColumnListView.h"

#include "globals.h"
#include "dialvat.h"
#include "fakdata.h"	// toint...
#include <stdio.h>

const uint32 BUT_NEW	= 'DVBN';
const uint32 BUT_DEL	= 'DVBD';
const uint32 BUT_SAVE	= 'DVBS';
const uint32 DC			= 'DVDC';
const uint32 LIST_INV	= 'DVLI';
const uint32 LIST_SEL	= 'DVLS';

dialVat::dialVat(sqlite *db, BHandler *hr) : BWindow(
	BRect(120, 120, 120+380, 120+250),
	"Stawki VAT",
	B_TITLED_WINDOW,
	B_NOT_RESIZABLE ), beFakTab(NULL, db, hr) {

	this->SetFeel(B_FLOATING_APP_WINDOW_FEEL);

	view = new BView(this->Bounds(), "vatView", B_FOLLOW_ALL_SIDES, 0);
	view->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	this->AddChild(view);

	CLVContainerView *containerView;
	list = new ColumnListView(BRect(10,10,120,175), &containerView, NULL, B_FOLLOW_LEFT|B_FOLLOW_TOP_BOTTOM,
		B_WILL_DRAW|B_FRAME_EVENTS|B_NAVIGABLE, B_SINGLE_SELECTION_LIST, false, true, true, true,
		B_FANCY_BORDER);
	list->AddColumn(new CLVColumn("Nazwa", 55, CLV_TELL_ITEMS_WIDTH|CLV_HEADER_TRUNCATE|CLV_SORT_KEYABLE));
	list->AddColumn(new CLVColumn("Stawka", 54, CLV_TELL_ITEMS_WIDTH|CLV_HEADER_TRUNCATE|CLV_SORT_KEYABLE));
	list->SetSortFunction(CLVEasyItem::CompareItems);
	this->view->AddChild(containerView);
	list->SetInvocationMessage(new BMessage(LIST_INV));
	list->SetSelectionMessage(new BMessage(LIST_SEL));

	box1 = new BBox(BRect(150,45,350,130), "dVBox1");
	box1->SetLabel("Dane stawki podatku");
	view->AddChild(box1);

	name  = new BTextControl(BRect(10,20,160,40), "dVNazwa", "Nazwa", NULL, new BMessage(DC));
	stawka = new BTextControl(BRect(10,50,160,70), "dvStawka", "Stawka (%)", NULL, new BMessage(DC));
	box1->AddChild(name);
	box1->AddChild(stawka);

	but_new = new BButton(BRect(160,200,240,230), "dv_but_new", "Nowa", new BMessage(BUT_NEW), B_FOLLOW_BOTTOM);
	but_del = new BButton(BRect(30,200,110,230), "dv_but_del", "Usuń", new BMessage(BUT_DEL), B_FOLLOW_LEFT|B_FOLLOW_BOTTOM);
	but_save = new BButton(BRect(290,200,370,230), "dv_but_save", "Zapisz", new BMessage(BUT_SAVE), B_FOLLOW_RIGHT|B_FOLLOW_BOTTOM);
	view->AddChild(but_new);
	view->AddChild(but_del);
	view->AddChild(but_save);
	but_new->ResizeToPreferred();
	but_del->ResizeToPreferred();
	but_save->ResizeToPreferred();

	// fix widths
	float d = max(be_plain_font->StringWidth(name->Label())+5,be_plain_font->StringWidth(stawka->Label())+5);
	name->SetDivider(d); stawka->SetDivider(d);

	makeNewStawka();
	RefreshIndexList();
	Show();
}

dialVat::~dialVat() {

}

bool dialVat::QuitRequested(void) {
	// refresh vat menus everywhere
	BMessage *msg;
	msg = new BMessage(MSG_REQVATUP);
	handler->Looper()->PostMessage(msg);
	return true;
}

void dialVat::makeNewStawka(void) {
	id = -1;
	this->dirty = false;
	list->DeselectAll();
	name->SetText(""); stawka->SetText("");
	updateTab();
}

void dialVat::updateTab(void) {
	bool state = (id<0);
	stawka->SetText(validateDecimal(stawka->Text()));
	stawka->SetEnabled(state);
}

void dialVat::MessageReceived(BMessage *Message) {
	BString sql;
	int ret;

	switch (Message->what) {
		case DC:
			this->dirty = true;
			break;
		case BUT_NEW:
			makeNewStawka();
			break;
		case BUT_SAVE:
			if (id>=0) {
				sql = "UPDATE stawka_vat SET name = %Q WHERE aktywne = 1 AND id = "; sql << id;
				ret = sqlite_exec_printf(dbData, sql.String(), 0, 0, &dbErrMsg, name->Text());
			} else {
				// dopisujemy nowy, ostrzec ze nieodwracalnie???
				if (strlen(name->Text())>0) {
					stawka->SetText(validateDecimal(stawka->Text()));
					ret = sqlite_exec_printf(dbData,
					"INSERT INTO stawka_vat (name,stawka,aktywne) VALUES ( %Q, %Q, 1 )",
					0, 0, &dbErrMsg, name->Text(), stawka->Text());
				}
			}
			RefreshIndexList();
			break;
		case BUT_DEL:
			if (id>=0) {
				sql = "UPDATE stawka_vat SET aktywne = 0 WHERE id = "; sql << id;
				ret = sqlite_exec_printf(dbData, sql.String(), 0, 0, &dbErrMsg);
			}
			makeNewStawka();
			RefreshIndexList();
			break;
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
			BWindow::MessageReceived(Message);
			break;
	}
}

void dialVat::ChangedSelection(int newid) {
	// fetch and store new data
	id = newid;
	if (id >=0) {
		int nRows, nCols;
		char **result;
		BString sql = "SELECT name, stawka FROM stawka_vat WHERE id = "; sql << id;
		sqlite_get_table(dbData, sql.String(), &result, &nRows, &nCols, &dbErrMsg);
		if (nRows < 1) {
			// no entries
		} else {
			name->SetText(result[nCols]);
			stawka->SetText(validateDecimal(result[nCols+1]));
		}
		sqlite_free_table(result);
		this->dirty = false;
		updateTab();
	}
}

void dialVat::RefreshIndexList(void) {
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
	sqlite_get_table(dbData, "SELECT id, name, stawka FROM stawka_vat WHERE aktywne = 1 ORDER BY id", &result, &nRows, &nCols, &dbErrMsg);
	if (nRows < 1) {
		// no entries
	} else {
		for (int i=1;i<=nRows;i++)
			list->AddItem(new tab2ListItem(toint(result[i*nCols+0]), result[i*nCols+1], result[i*nCols+2]));
	}
	sqlite_free_table(result);
}
