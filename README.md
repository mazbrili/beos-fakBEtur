
fakBEtur 0.5 (20061228)
=======================

(c) Maciej Witkowiak <mwitkowiak@gmail.com>, <ytm@elysium.pl>
http://ytm.bossstation.dnsalias.org/html/beos.html
http://members.elysium.pl/html/beos.html

fakBEtur is a program supporting invoicing. The program uses its own database in which it stores information about the issued documents. The database is also used to store data of contractors and goods.

#SCREEN SHOTS
![Screenshot](/images/fakbetur-01.png?raw=true) 
![Screenshot](/images/fakbetur-02.png?raw=true) 
![Screenshot](/images/fakbetur-03.png?raw=true) 
![Screenshot](/images/fakbetur-04.png?raw=true) 
![Screenshot](/images/fakbetur-05.png?raw=true) 
![Screenshot](/images/fakbetur-06.png?raw=true) 
![Screenshot](/images/fakbetur-07.png?raw=true) 
![Screenshot](/images/fakbetur-08.png?raw=true) 
![Screenshot](/images/fakbetur-09.png?raw=true) 
![Screenshot](/images/fakbetur-0a.png?raw=true) 


#Why OPEN SOURCE
Release 0.5 of the program has been made available along with the source code in the hope that it will prove useful.
I myself do not intend to develop the program further (the to-do list in the TODO file), I miss it
enthusiasm and even more so time. Creating a program of this type without solid knowledge and / or support
substantive on the part of users does not make sense. For me, it ceased to be a job, by the way
which you can learn something and turn into a job.


#LICENSE
The program's source code was released in the hope that it would be useful. You can copy it, distribute it, change it, and create new programs (also commercial) on its basis.
The only thing that is not allowed in the case of use is to state the origin and authorship of the code. It would be
politely inform me and boast about what this code was used for.
In the case of paid applications, I count on a free copy.

The author has made every effort, but does not give any guarantee for correct operation
the program, nor the documents generated by the program.

For legal professionals - the attached license is the MIT License.


#CHANGES
### 0.5
 - Segfault removed when calculating margin at zero net price
### 0.4
 - in the case of incorrect data, the focus is moved to the appropriate field
 - the ability to import a list of items from another document
 - monthly sales summary
 - printout original / copies, original + copies, possibility to configure the number of copies
 - configuration of the default number of days for payment (from the day of sale)
 - new support for lists, the ability to sort by selected columns
 - dialogue with the calendar to choose the date
 - report on overdue receivables
 - the ability to set a default value for the 'Issued' field
 - option - simplified document numbering (no month)
 - uniform keyboard shortcuts: F2 / 3/4 - tab change, F5 / 6/8/9 - action, ENTER - write
 - summary on the tab with the list of invoice items
 - report on paid / unpaid contractor invoices, available from receivables and companies tab
 - 'paid' button for immediate payment of the entire invoice
 - information about goods extended with the inventory and the last shift
### 0.3
 - database format change, database versioning added - currently no upgrade possible,
   it will be possible in the event of subsequent changes
 - path saving (save directory, HTML template)
 - the goods / companies appearing on the invoice for the first time are added to the database
 - possibility to edit tax rates
 - full print configuration (type of export, number of copies, type of document)
 - after adding the goods to the invoice, the focus is moved to the "Quantity" field
 - printout via print_server (default font is Arial, for printout to pdf it should be in options
   embed it in the file)
### 0.2
 - resignation from SpLocale
 - data validation
 - better HTML export template (table with frames)
 - refreshing symbols in the invoice tab after changes in the database
 - names of all fields depending on the current font, always visible
 - automatic setting of a new invoice number, place of issue set to
   company headquarters
### 0.1
 - first public release


#REQUIREMENTS
The program requires BeOS to be configured to support the Polish keyboard layout and
installation of fonts with Polish characters.
A template is required for HTML export, a sample template is included with the
program. It can be placed in any location.



#INSTALLATION
The program can be unpacked anywhere, a template is required for HTML export.
The template should be placed by default in the file: /boot/home/vatszablon.html, if not there
will, the program will ask you to select a template. Its location will be remembered.



#CONFIGURATION
At the first start, the program will create a database and set the default configuration.
Then a dialogue will be opened in which you should provide the data of the company issuing the invoices.



#SERVICE
Most of the operations and switching between tabs can be performed with the function keys.
The 'ENTER' key is equivalent to saving the current card into the database. Attention! On the tab
invoices, do not confuse the entry of the goods on the invoice with the entry of the entire invoice.

The 'SYMBOL' field in the list of contractors and items is used to enter a shortened form
name, thanks to which it is possible to quickly find records and fill in invoice fields.
The 'Name' and 'SYMBOL' fields must be filled in and unique on all tabs.

You can enter simple arithmetic operations in numeric fields, but to avoid them
incorrect rounding of numbers should always be entered as a decimal.
For example, instead of "3/7" you would enter "3.0 / 7.0".

## a) MENU
###DOCUMENT
Selects the type of documents to be printed.

###OPTIONS
Here you can select the printing / export method, change company parameters, and define
new tax rates and default values - the number of printed copies and the due date.
In addition, you can turn on simplified numbering of invoices - without a month. When editing existing ones
tax rates, it is not possible to change the rate, only change the name. This allows you to keep
data integrity.

### SUMMARY
This menu gives you summaries of monthly sales and outstanding balances. Current receivables
each contractor can be recalled on the contractor's card. A negative number of days to pay (you can
also enter it in the receivables dialogue) means how many days are left until the arrears arise.

## b) GOODS
A simple calculator for calculating prices has been placed in the 'Goods' tab.
The purpose of the buttons:
- Sales price - calculates the new net selling price based on the purchase price and the discount
- Margin - calculates the margin on the basis of the purchase price, discount and net sale price
- Import - based on the exchange rate, purchase price (the unit is then a foreign currency),
  duty, margin and rebate calculates the net selling price
You can also change the current stock level on this tab.

## c) CONTRACTORS
The customer / vendor fields have no meaning at the moment.
The 'Active' field controls whether the contractor's symbol is to be selectable in the tab
invoices.
The 'Blocked' field blocks the contractor's data from being edited and prevents accidental ones
changes.

## c) INVOICES
In the 'Payment' fields, you can select / enter the date of payment or the number of days from the date of sale.
In the latter case, the date will be updated automatically when the focus is moved to another
a text field (e.g. with a tab).
When adding / editing items, always click 'Save' (in the invoice item frame),
to add new data to the invoice, and then press ENTER or click 'Save' at the bottom,
to save all invoice information.
The "Import from another" button is used to copy a list of items from another document to the current one.
Attention! If we edit a document that has a sale date prior to the
the last warehouse change of a given item, the stock level will not change.



#PRINT
To print a document, select the type of document from the 'Document' menu and
printing method from the 'Options' menu, then go to the 'INVOICES' tab, select
invoice from the list and click 'Print' at the bottom or press F9.



#HTML EXPORT
Any changes to the appearance can be made using the sample template. Texts
surrounded by @ signs, e.g. @ FNAME @ will be converted to the appropriate data from
invoices. The @ TABLE @ field will be converted to a string of HTML table rows, with no additional attributes, as will the @ TABSUM @ field.



#EXPORT TXT
Writing to a text file serves as a demonstration of the capabilities rather than actually useful
option. Properly processed, it can be a base for direct printing on dot matrix printers.



#ARCHIVIZATION
To make a database archive it is enough to save the file in a safe way
databases: /boot/home/config/share/fakbetur.db



#DEINSTALLATION
Remove the program, HTML template file and database file
/boot/home/config/share/fakbetur.db
