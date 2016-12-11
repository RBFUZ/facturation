WARNINGS=-Wall -Wimplicit -Wunused -Wunused-result -Wshadow -Wconversion -Wfloat-equal -Wparentheses -Wundef -Wextra --std=c89 -Wstrict-prototypes -Wwrite-strings -Wconversion -fdiagnostics-show-option -Werror ${EXTRA_WARNINGS}
CFLAGS= ${WARNINGS} -Iinclude -fPIC
RELEASE_CFLAGS= -Wuninitialized -DNDEBUG 
DEBUG_CFLAGS= -g3 -ggdb3 
GTK_CFLAGS=`pkg-config gtk+-2.0 --cflags`
GTK_LIBS=`pkg-config gtk+-2.0 --libs`


all: debug/facturation release/facturation

clean:
	rm -rf debug release unittest forstudent

debug/facturation: provided/libprovideddebug.so debug/CatalogRecordEditor.c.o debug/CustomerRecordEditor.c.o debug/App.c.o debug/Bill.c.o debug/Catalog.c.o debug/CatalogDB.c.o debug/CatalogDBUnit.c.o debug/CatalogRecord.c.o debug/CatalogRecordUnit.c.o debug/Customer.c.o debug/CustomerDB.c.o debug/CustomerDBUnit.c.o debug/CustomerRecord.c.o debug/CustomerRecordUnit.c.o debug/Dictionary.c.o debug/DictionaryUnit.c.o debug/Document.c.o debug/DocumentEditor.c.o debug/DocumentRowList.c.o debug/DocumentRowListUnit.c.o debug/DocumentUnit.c.o debug/DocumentUtil.c.o debug/DocumentUtilUnit.c.o debug/EncryptDecrypt.c.o debug/EncryptDecryptUnit.c.o debug/GtkCatalogModel.c.o debug/GtkCustomerModel.c.o debug/main.c.o debug/MainWindow.c.o debug/MyString.c.o debug/MyStringUnit.c.o debug/Operator.c.o debug/OperatorTable.c.o debug/OperatorTableUnit.c.o debug/Print.c.o debug/PrintFormat.c.o debug/PrintFormatUnit.c.o debug/Quotation.c.o
	@mkdir -p debug
	LANG=C gcc -o debug/facturation debug/CatalogRecordEditor.c.o debug/CustomerRecordEditor.c.o debug/App.c.o debug/Bill.c.o debug/Catalog.c.o debug/CatalogDB.c.o debug/CatalogDBUnit.c.o debug/CatalogRecord.c.o debug/CatalogRecordUnit.c.o debug/Customer.c.o debug/CustomerDB.c.o debug/CustomerDBUnit.c.o debug/CustomerRecord.c.o debug/CustomerRecordUnit.c.o debug/Dictionary.c.o debug/DictionaryUnit.c.o debug/Document.c.o debug/DocumentEditor.c.o debug/DocumentRowList.c.o debug/DocumentRowListUnit.c.o debug/DocumentUnit.c.o debug/DocumentUtil.c.o debug/DocumentUtilUnit.c.o debug/EncryptDecrypt.c.o debug/EncryptDecryptUnit.c.o debug/GtkCatalogModel.c.o debug/GtkCustomerModel.c.o debug/main.c.o debug/MainWindow.c.o debug/MyString.c.o debug/MyStringUnit.c.o debug/Operator.c.o debug/OperatorTable.c.o debug/OperatorTableUnit.c.o debug/Print.c.o debug/PrintFormat.c.o debug/PrintFormatUnit.c.o debug/Quotation.c.o -Wl,-rpath=provided:../provided ${GTK_LIBS} -Lprovided -lprovideddebug -lm 	
	mkdir -p /tmp/facturation/data || true
	cp printformat/* /tmp/facturation/data/ || true

release/facturation: provided/libprovidedrelease.so release/CatalogRecordEditor.c.o release/CustomerRecordEditor.c.o release/App.c.o release/Bill.c.o release/Catalog.c.o release/CatalogDB.c.o release/CatalogDBUnit.c.o release/CatalogRecord.c.o release/CatalogRecordUnit.c.o release/Customer.c.o release/CustomerDB.c.o release/CustomerDBUnit.c.o release/CustomerRecord.c.o release/CustomerRecordUnit.c.o release/Dictionary.c.o release/DictionaryUnit.c.o release/Document.c.o release/DocumentEditor.c.o release/DocumentRowList.c.o release/DocumentRowListUnit.c.o release/DocumentUnit.c.o release/DocumentUtil.c.o release/DocumentUtilUnit.c.o release/EncryptDecrypt.c.o release/EncryptDecryptUnit.c.o release/GtkCatalogModel.c.o release/GtkCustomerModel.c.o release/main.c.o release/MainWindow.c.o release/MyString.c.o release/MyStringUnit.c.o release/Operator.c.o release/OperatorTable.c.o release/OperatorTableUnit.c.o release/Print.c.o release/PrintFormat.c.o release/PrintFormatUnit.c.o release/Quotation.c.o
	@mkdir -p release
	LANG=C gcc -o release/facturation release/CatalogRecordEditor.c.o release/CustomerRecordEditor.c.o release/App.c.o release/Bill.c.o release/Catalog.c.o release/CatalogDB.c.o release/CatalogDBUnit.c.o release/CatalogRecord.c.o release/CatalogRecordUnit.c.o release/Customer.c.o release/CustomerDB.c.o release/CustomerDBUnit.c.o release/CustomerRecord.c.o release/CustomerRecordUnit.c.o release/Dictionary.c.o release/DictionaryUnit.c.o release/Document.c.o release/DocumentEditor.c.o release/DocumentRowList.c.o release/DocumentRowListUnit.c.o release/DocumentUnit.c.o release/DocumentUtil.c.o release/DocumentUtilUnit.c.o release/EncryptDecrypt.c.o release/EncryptDecryptUnit.c.o release/GtkCatalogModel.c.o release/GtkCustomerModel.c.o release/main.c.o release/MainWindow.c.o release/MyString.c.o release/MyStringUnit.c.o release/Operator.c.o release/OperatorTable.c.o release/OperatorTableUnit.c.o release/Print.c.o release/PrintFormat.c.o release/PrintFormatUnit.c.o release/Quotation.c.o -Wl,-rpath=provided:../provided ${GTK_LIBS} -Lprovided -lprovidedrelease -lm 
	mkdir -p /tmp/facturation/data || true
	cp printformat/* /tmp/facturation/data/ || true




