
#include "fakdata.h"

#include <stdio.h>

firmadat::firmadat(sqlite *db) {
	clear();
	dbData = db;
}

void firmadat::dump(void) {
	int i;
	for (i=0;i<=10;i++) {
		printf("%i:[%s] ",i,this->data[i].String());
	}
	printf("\n,odb:%i,dos:%i,akt:%i,zab:%i\n-----\n",odbiorca,dostawca,aktywny,zablokowany);
}

void firmadat::clear(void) {
	id = -1;
	for (int i=0;i<=10;i++) {
		data[i] = "";
	}
	odbiorca = dostawca = aktywny = zablokowany = false;
}

int firmadat::generate_id(void) {
	int newid = 1;
	int nRows, nCols;
	char **result;
	sqlite_get_table(dbData, "SELECT MAX(id) FROM firma", &result, &nRows, &nCols, &dbErrMsg);
	if (nRows > 0) {
		// there is something in db
		newid = toint(result[1]) + 1;
	}
	sqlite_free_table(result);
	return newid;
}

void firmadat::commit(void) {
	BString sql;
	int ret;
printf("commit\n");
	if (id>=0) {	// UPDATE
		sql = "UPDATE firma SET ";
		sql += "nazwa = %Q, symbol = %Q, adres = %Q, kod = %Q, miejscowosc = %Q, telefon = %Q, email = %Q";
		sql += ", nip = %Q, regon = %Q, bank = %Q, konto = %Q";
		sql += ", odbiorca = %i, dostawca = %i, aktywny = %i, zablokowany = %i";
		sql += " WHERE id = %i";
	} else {		// INSERT
		id = generate_id();
		sql += "INSERT INTO firma ( ";
		sql += "nazwa, symbol, adres, kod, miejscowosc, telefon, email";
		sql += ", nip, regon, bank, konto";
		sql += ", odbiorca, dostawca, aktywny, zablokowany";
		sql += ", id ) VALUES ( ";
		sql += "%Q, %Q, %Q, %Q, %Q, %Q, %Q";
		sql += ", %Q, %Q, %Q, %Q";
		sql += ", %i, %i, %i, %i";
		sql += ", %i)";
	}
//printf("sql:[%s]\n",sql.String());
	ret = sqlite_exec_printf(dbData, sql.String(), 0, 0, &dbErrMsg,
		data[0].String(), data[1].String(), data[2].String(), data[3].String(),
		data[4].String(), data[5].String(), data[6].String(), data[7].String(),
		data[8].String(), data[9].String(), data[10].String(),
		odbiorca, dostawca, aktywny, zablokowany,
		id);
	printf("result: %i, %s; id=%i\n", ret, dbErrMsg, id);
}

void firmadat::fetch(void) {
printf("in fetchcurdata with %i\n",id);
	int i, j;
	int nRows, nCols;
	char **result;
	BString sql;	
	sql = "SELECT ";
	sql += "nazwa, symbol, adres, kod, miejscowosc, telefon, email";
	sql += ", nip, regon, bank, konto";
	sql += ", odbiorca, dostawca, aktywny, zablokowany";
	sql += " FROM firma WHERE id = ";
	sql << id;
//printf("sql:%s\n",sql.String());
	sqlite_get_table(dbData, sql.String(), &result, &nRows, &nCols, &dbErrMsg);
printf ("got:%ix%i\n", nRows, nCols);
	// readout data
	i = nCols;
	for (j=0;j<=10;j++) {
		data[j] = result[i++];
	}
	odbiorca = toint(result[i++]);
	dostawca = toint(result[i++]);
	aktywny = toint(result[i++]);
	zablokowany = toint(result[i++]);

	sqlite_free_table(result);
}

void firmadat::del(void) {
	if (id>=0) {
		sqlite_exec_printf(dbData, "DELETE FROM firma WHERE id = %i", 0, 0, &dbErrMsg, id);
	}
	clear();
}

//----------------------

towardat::towardat(sqlite *db) {
	clear();
	dbData = db;
}

void towardat::dump(void) {
	printf("towardat: id=%i, implement rest\n", id);
}

void towardat::clear(void) {
	id = -1;
	data[0] = data[1] = data[2] = data[3] = notatki = "";
	usluga = false;
	netto = zakupu = marza = rabat = 0;
}

int towardat::generate_id(void) {
	int newid = 1;
	int nRows, nCols;
	char **result;
	sqlite_get_table(dbData, "SELECT MAX(id) FROM towar", &result, &nRows, &nCols, &dbErrMsg);
	if (nRows > 0) {
		// there is something in db
		newid = toint(result[1]) + 1;
	}
	sqlite_free_table(result);
	return newid;
}

//----------------------

#include <stdlib.h>

int toint(const char *input) {
	if (input != NULL)
		return strtol(input, NULL, 10);
	else
		return 0;
}
