
#ifndef _globals_h
#define _globals_h
	// application signature
	#define APP_NAME "fakBEtur"
	// bump it on every release
	#define APP_VERSION "0.5"
	// sync it to sqlschema CVS revision
	#define APP_DBVERSION "1.6"
	#define APP_SIGNATURE "application/x-vnd.generic-fakBEtur"
	extern int AppReturnValue;
	// general max() function
	#define max(x,y) ( ((x)>(y)) ? (x) : (y) )
	// gettext-style translation helper
	#define _(x) SpTranslate(x)
	// Qt-style translation helper
	#define tr(x) SpTranslate(x)
	// db file with full path
	#define DATABASE_PATHNAME "/boot/home/config/settings/fakbetur.db"

	// some globally visible stuff
	#define MSG_REQTOWARUP		'RQTU'	// symbols in invoice
	#define MSG_REQFIRMAUP		'RQFU'	// symbols in invoice
	#define MSG_REQTOWARLIST	'RQTL'	// refresh commodity list
	#define MSG_REQFIRMALIST	'RQFL'	// refresh firma list
	#define MSG_REQFAKPOZLIST	'RQFT'	// refresh commodity list on invoice, make dirty
	#define MSG_REQVATUP		'RQVU'	// refresh vat menu - invoice, commodity
	#define MENU_PAGESETUP		'MPRS'	// in invoice - call printsetup
	#define MENU_PRINTPAGE		'MPRP'	// by invoice's button only?
#endif
