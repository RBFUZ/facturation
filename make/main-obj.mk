release/CatalogRecordEditor.c.o: src/CatalogRecordEditor.c
	@mkdir -p release
		LANG=C gcc -c ${CFLAGS} ${RELEASE_CFLAGS} ${GTK_CFLAGS} -o release/CatalogRecordEditor.c.o src/CatalogRecordEditor.c

debug/CatalogRecordEditor.c.o: src/CatalogRecordEditor.c
	@mkdir -p debug
	LANG=C gcc -c ${CFLAGS} ${DEBUG_CFLAGS} ${GTK_CFLAGS} -o debug/CatalogRecordEditor.c.o src/CatalogRecordEditor.c

release/CustomerRecordEditor.c.o: src/CustomerRecordEditor.c
	@mkdir -p release
		LANG=C gcc -c ${CFLAGS} ${RELEASE_CFLAGS} ${GTK_CFLAGS} -o release/CustomerRecordEditor.c.o src/CustomerRecordEditor.c

debug/CustomerRecordEditor.c.o: src/CustomerRecordEditor.c
	@mkdir -p debug
	LANG=C gcc -c ${CFLAGS} ${DEBUG_CFLAGS} ${GTK_CFLAGS} -o debug/CustomerRecordEditor.c.o src/CustomerRecordEditor.c


release/App.c.o: src/App.c
	@mkdir -p release
		LANG=C gcc -c ${CFLAGS} ${RELEASE_CFLAGS} ${GTK_CFLAGS} -o release/App.c.o src/App.c

debug/App.c.o: src/App.c
	@mkdir -p debug
	LANG=C gcc -c ${CFLAGS} ${DEBUG_CFLAGS} ${GTK_CFLAGS} -o debug/App.c.o src/App.c

release/Bill.c.o: src/Bill.c
	@mkdir -p release
		LANG=C gcc -c ${CFLAGS} ${RELEASE_CFLAGS} ${GTK_CFLAGS} -o release/Bill.c.o src/Bill.c

debug/Bill.c.o: src/Bill.c
	@mkdir -p debug
	LANG=C gcc -c ${CFLAGS} ${DEBUG_CFLAGS} ${GTK_CFLAGS} -o debug/Bill.c.o src/Bill.c

release/BridgeUtil.c.o: src/BridgeUtil.c
	@mkdir -p release
		LANG=C gcc -c ${CFLAGS} ${RELEASE_CFLAGS} ${GTK_CFLAGS} -o release/BridgeUtil.c.o src/BridgeUtil.c

debug/BridgeUtil.c.o: src/BridgeUtil.c
	@mkdir -p debug
	LANG=C gcc -c ${CFLAGS} ${DEBUG_CFLAGS} ${GTK_CFLAGS} -o debug/BridgeUtil.c.o src/BridgeUtil.c

release/Catalog.c.o: src/Catalog.c
	@mkdir -p release
		LANG=C gcc -c ${CFLAGS} ${RELEASE_CFLAGS} ${GTK_CFLAGS} -o release/Catalog.c.o src/Catalog.c

debug/Catalog.c.o: src/Catalog.c
	@mkdir -p debug
	LANG=C gcc -c ${CFLAGS} ${DEBUG_CFLAGS} ${GTK_CFLAGS} -o debug/Catalog.c.o src/Catalog.c

release/CatalogDB.c.o: src/CatalogDB.c
	@mkdir -p release
		LANG=C gcc -c ${CFLAGS} ${RELEASE_CFLAGS} ${GTK_CFLAGS} -o release/CatalogDB.c.o src/CatalogDB.c

debug/CatalogDB.c.o: src/CatalogDB.c
	@mkdir -p debug
	LANG=C gcc -c ${CFLAGS} ${DEBUG_CFLAGS} ${GTK_CFLAGS} -o debug/CatalogDB.c.o src/CatalogDB.c

release/CatalogDBUnit.c.o: src/CatalogDBUnit.c
	@mkdir -p release
		LANG=C gcc -c ${CFLAGS} ${RELEASE_CFLAGS} ${GTK_CFLAGS} -o release/CatalogDBUnit.c.o src/CatalogDBUnit.c

debug/CatalogDBUnit.c.o: src/CatalogDBUnit.c
	@mkdir -p debug
	LANG=C gcc -c ${CFLAGS} ${DEBUG_CFLAGS} ${GTK_CFLAGS} -o debug/CatalogDBUnit.c.o src/CatalogDBUnit.c

release/CatalogRecord.c.o: src/CatalogRecord.c
	@mkdir -p release
		LANG=C gcc -c ${CFLAGS} ${RELEASE_CFLAGS} ${GTK_CFLAGS} -o release/CatalogRecord.c.o src/CatalogRecord.c

debug/CatalogRecord.c.o: src/CatalogRecord.c
	@mkdir -p debug
	LANG=C gcc -c ${CFLAGS} ${DEBUG_CFLAGS} ${GTK_CFLAGS} -o debug/CatalogRecord.c.o src/CatalogRecord.c

release/CatalogRecordUnit.c.o: src/CatalogRecordUnit.c
	@mkdir -p release
		LANG=C gcc -c ${CFLAGS} ${RELEASE_CFLAGS} ${GTK_CFLAGS} -o release/CatalogRecordUnit.c.o src/CatalogRecordUnit.c

debug/CatalogRecordUnit.c.o: src/CatalogRecordUnit.c
	@mkdir -p debug
	LANG=C gcc -c ${CFLAGS} ${DEBUG_CFLAGS} ${GTK_CFLAGS} -o debug/CatalogRecordUnit.c.o src/CatalogRecordUnit.c

release/Customer.c.o: src/Customer.c
	@mkdir -p release
		LANG=C gcc -c ${CFLAGS} ${RELEASE_CFLAGS} ${GTK_CFLAGS} -o release/Customer.c.o src/Customer.c

debug/Customer.c.o: src/Customer.c
	@mkdir -p debug
	LANG=C gcc -c ${CFLAGS} ${DEBUG_CFLAGS} ${GTK_CFLAGS} -o debug/Customer.c.o src/Customer.c

release/CustomerDB.c.o: src/CustomerDB.c
	@mkdir -p release
		LANG=C gcc -c ${CFLAGS} ${RELEASE_CFLAGS} ${GTK_CFLAGS} -o release/CustomerDB.c.o src/CustomerDB.c

debug/CustomerDB.c.o: src/CustomerDB.c
	@mkdir -p debug
	LANG=C gcc -c ${CFLAGS} ${DEBUG_CFLAGS} ${GTK_CFLAGS} -o debug/CustomerDB.c.o src/CustomerDB.c

release/CustomerDBUnit.c.o: src/CustomerDBUnit.c
	@mkdir -p release
		LANG=C gcc -c ${CFLAGS} ${RELEASE_CFLAGS} ${GTK_CFLAGS} -o release/CustomerDBUnit.c.o src/CustomerDBUnit.c

debug/CustomerDBUnit.c.o: src/CustomerDBUnit.c
	@mkdir -p debug
	LANG=C gcc -c ${CFLAGS} ${DEBUG_CFLAGS} ${GTK_CFLAGS} -o debug/CustomerDBUnit.c.o src/CustomerDBUnit.c

release/CustomerRecord.c.o: src/CustomerRecord.c
	@mkdir -p release
		LANG=C gcc -c ${CFLAGS} ${RELEASE_CFLAGS} ${GTK_CFLAGS} -o release/CustomerRecord.c.o src/CustomerRecord.c

debug/CustomerRecord.c.o: src/CustomerRecord.c
	@mkdir -p debug
	LANG=C gcc -c ${CFLAGS} ${DEBUG_CFLAGS} ${GTK_CFLAGS} -o debug/CustomerRecord.c.o src/CustomerRecord.c

release/CustomerRecordUnit.c.o: src/CustomerRecordUnit.c
	@mkdir -p release
		LANG=C gcc -c ${CFLAGS} ${RELEASE_CFLAGS} ${GTK_CFLAGS} -o release/CustomerRecordUnit.c.o src/CustomerRecordUnit.c

debug/CustomerRecordUnit.c.o: src/CustomerRecordUnit.c
	@mkdir -p debug
	LANG=C gcc -c ${CFLAGS} ${DEBUG_CFLAGS} ${GTK_CFLAGS} -o debug/CustomerRecordUnit.c.o src/CustomerRecordUnit.c

release/Dictionary.c.o: src/Dictionary.c
	@mkdir -p release
		LANG=C gcc -c ${CFLAGS} ${RELEASE_CFLAGS} ${GTK_CFLAGS} -o release/Dictionary.c.o src/Dictionary.c

debug/Dictionary.c.o: src/Dictionary.c
	@mkdir -p debug
	LANG=C gcc -c ${CFLAGS} ${DEBUG_CFLAGS} ${GTK_CFLAGS} -o debug/Dictionary.c.o src/Dictionary.c

release/DictionaryUnit.c.o: src/DictionaryUnit.c
	@mkdir -p release
		LANG=C gcc -c ${CFLAGS} ${RELEASE_CFLAGS} ${GTK_CFLAGS} -o release/DictionaryUnit.c.o src/DictionaryUnit.c

debug/DictionaryUnit.c.o: src/DictionaryUnit.c
	@mkdir -p debug
	LANG=C gcc -c ${CFLAGS} ${DEBUG_CFLAGS} ${GTK_CFLAGS} -o debug/DictionaryUnit.c.o src/DictionaryUnit.c

release/Document.c.o: src/Document.c
	@mkdir -p release
		LANG=C gcc -c ${CFLAGS} ${RELEASE_CFLAGS} ${GTK_CFLAGS} -o release/Document.c.o src/Document.c

debug/Document.c.o: src/Document.c
	@mkdir -p debug
	LANG=C gcc -c ${CFLAGS} ${DEBUG_CFLAGS} ${GTK_CFLAGS} -o debug/Document.c.o src/Document.c

release/DocumentEditor.c.o: src/DocumentEditor.c
	@mkdir -p release
		LANG=C gcc -c ${CFLAGS} ${RELEASE_CFLAGS} ${GTK_CFLAGS} -o release/DocumentEditor.c.o src/DocumentEditor.c

debug/DocumentEditor.c.o: src/DocumentEditor.c
	@mkdir -p debug
	LANG=C gcc -c ${CFLAGS} ${DEBUG_CFLAGS} ${GTK_CFLAGS} -o debug/DocumentEditor.c.o src/DocumentEditor.c

release/DocumentRowList.c.o: src/DocumentRowList.c
	@mkdir -p release
		LANG=C gcc -c ${CFLAGS} ${RELEASE_CFLAGS} ${GTK_CFLAGS} -o release/DocumentRowList.c.o src/DocumentRowList.c

debug/DocumentRowList.c.o: src/DocumentRowList.c
	@mkdir -p debug
	LANG=C gcc -c ${CFLAGS} ${DEBUG_CFLAGS} ${GTK_CFLAGS} -o debug/DocumentRowList.c.o src/DocumentRowList.c

release/DocumentRowListUnit.c.o: src/DocumentRowListUnit.c
	@mkdir -p release
		LANG=C gcc -c ${CFLAGS} ${RELEASE_CFLAGS} ${GTK_CFLAGS} -o release/DocumentRowListUnit.c.o src/DocumentRowListUnit.c

debug/DocumentRowListUnit.c.o: src/DocumentRowListUnit.c
	@mkdir -p debug
	LANG=C gcc -c ${CFLAGS} ${DEBUG_CFLAGS} ${GTK_CFLAGS} -o debug/DocumentRowListUnit.c.o src/DocumentRowListUnit.c

release/DocumentUnit.c.o: src/DocumentUnit.c
	@mkdir -p release
		LANG=C gcc -c ${CFLAGS} ${RELEASE_CFLAGS} ${GTK_CFLAGS} -o release/DocumentUnit.c.o src/DocumentUnit.c

debug/DocumentUnit.c.o: src/DocumentUnit.c
	@mkdir -p debug
	LANG=C gcc -c ${CFLAGS} ${DEBUG_CFLAGS} ${GTK_CFLAGS} -o debug/DocumentUnit.c.o src/DocumentUnit.c

release/DocumentUtil.c.o: src/DocumentUtil.c
	@mkdir -p release
		LANG=C gcc -c ${CFLAGS} ${RELEASE_CFLAGS} ${GTK_CFLAGS} -o release/DocumentUtil.c.o src/DocumentUtil.c

debug/DocumentUtil.c.o: src/DocumentUtil.c
	@mkdir -p debug
	LANG=C gcc -c ${CFLAGS} ${DEBUG_CFLAGS} ${GTK_CFLAGS} -o debug/DocumentUtil.c.o src/DocumentUtil.c

release/DocumentUtilUnit.c.o: src/DocumentUtilUnit.c
	@mkdir -p release
		LANG=C gcc -c ${CFLAGS} ${RELEASE_CFLAGS} ${GTK_CFLAGS} -o release/DocumentUtilUnit.c.o src/DocumentUtilUnit.c

debug/DocumentUtilUnit.c.o: src/DocumentUtilUnit.c
	@mkdir -p debug
	LANG=C gcc -c ${CFLAGS} ${DEBUG_CFLAGS} ${GTK_CFLAGS} -o debug/DocumentUtilUnit.c.o src/DocumentUtilUnit.c

release/EncryptDecrypt.c.o: src/EncryptDecrypt.c
	@mkdir -p release
		LANG=C gcc -c ${CFLAGS} ${RELEASE_CFLAGS} ${GTK_CFLAGS} -o release/EncryptDecrypt.c.o src/EncryptDecrypt.c

debug/EncryptDecrypt.c.o: src/EncryptDecrypt.c
	@mkdir -p debug
	LANG=C gcc -c ${CFLAGS} ${DEBUG_CFLAGS} ${GTK_CFLAGS} -o debug/EncryptDecrypt.c.o src/EncryptDecrypt.c

release/EncryptDecryptUnit.c.o: src/EncryptDecryptUnit.c
	@mkdir -p release
		LANG=C gcc -c ${CFLAGS} ${RELEASE_CFLAGS} ${GTK_CFLAGS} -o release/EncryptDecryptUnit.c.o src/EncryptDecryptUnit.c

debug/EncryptDecryptUnit.c.o: src/EncryptDecryptUnit.c
	@mkdir -p debug
	LANG=C gcc -c ${CFLAGS} ${DEBUG_CFLAGS} ${GTK_CFLAGS} -o debug/EncryptDecryptUnit.c.o src/EncryptDecryptUnit.c

release/GtkCatalogModel.c.o: src/GtkCatalogModel.c
	@mkdir -p release
		LANG=C gcc -c ${CFLAGS} ${RELEASE_CFLAGS} ${GTK_CFLAGS} -o release/GtkCatalogModel.c.o src/GtkCatalogModel.c

debug/GtkCatalogModel.c.o: src/GtkCatalogModel.c
	@mkdir -p debug
	LANG=C gcc -c ${CFLAGS} ${DEBUG_CFLAGS} ${GTK_CFLAGS} -o debug/GtkCatalogModel.c.o src/GtkCatalogModel.c

release/GtkCustomerModel.c.o: src/GtkCustomerModel.c
	@mkdir -p release
		LANG=C gcc -c ${CFLAGS} ${RELEASE_CFLAGS} ${GTK_CFLAGS} -o release/GtkCustomerModel.c.o src/GtkCustomerModel.c

debug/GtkCustomerModel.c.o: src/GtkCustomerModel.c
	@mkdir -p debug
	LANG=C gcc -c ${CFLAGS} ${DEBUG_CFLAGS} ${GTK_CFLAGS} -o debug/GtkCustomerModel.c.o src/GtkCustomerModel.c

release/main.c.o: src/main.c
	@mkdir -p release
		LANG=C gcc -c ${CFLAGS} ${RELEASE_CFLAGS} ${GTK_CFLAGS} -o release/main.c.o src/main.c

debug/main.c.o: src/main.c
	@mkdir -p debug
	LANG=C gcc -c ${CFLAGS} ${DEBUG_CFLAGS} ${GTK_CFLAGS} -o debug/main.c.o src/main.c

release/MainWindow.c.o: src/MainWindow.c
	@mkdir -p release
		LANG=C gcc -c ${CFLAGS} ${RELEASE_CFLAGS} ${GTK_CFLAGS} -o release/MainWindow.c.o src/MainWindow.c

debug/MainWindow.c.o: src/MainWindow.c
	@mkdir -p debug
	LANG=C gcc -c ${CFLAGS} ${DEBUG_CFLAGS} ${GTK_CFLAGS} -o debug/MainWindow.c.o src/MainWindow.c

release/MyString.c.o: src/MyString.c
	@mkdir -p release
		LANG=C gcc -c ${CFLAGS} ${RELEASE_CFLAGS} ${GTK_CFLAGS} -o release/MyString.c.o src/MyString.c

debug/MyString.c.o: src/MyString.c
	@mkdir -p debug
	LANG=C gcc -c ${CFLAGS} ${DEBUG_CFLAGS} ${GTK_CFLAGS} -o debug/MyString.c.o src/MyString.c

release/MyStringUnit.c.o: src/MyStringUnit.c
	@mkdir -p release
		LANG=C gcc -c ${CFLAGS} ${RELEASE_CFLAGS} ${GTK_CFLAGS} -o release/MyStringUnit.c.o src/MyStringUnit.c

debug/MyStringUnit.c.o: src/MyStringUnit.c
	@mkdir -p debug
	LANG=C gcc -c ${CFLAGS} ${DEBUG_CFLAGS} ${GTK_CFLAGS} -o debug/MyStringUnit.c.o src/MyStringUnit.c

release/Operator.c.o: src/Operator.c
	@mkdir -p release
		LANG=C gcc -c ${CFLAGS} ${RELEASE_CFLAGS} ${GTK_CFLAGS} -o release/Operator.c.o src/Operator.c

debug/Operator.c.o: src/Operator.c
	@mkdir -p debug
	LANG=C gcc -c ${CFLAGS} ${DEBUG_CFLAGS} ${GTK_CFLAGS} -o debug/Operator.c.o src/Operator.c

release/OperatorTable.c.o: src/OperatorTable.c
	@mkdir -p release
		LANG=C gcc -c ${CFLAGS} ${RELEASE_CFLAGS} ${GTK_CFLAGS} -o release/OperatorTable.c.o src/OperatorTable.c

debug/OperatorTable.c.o: src/OperatorTable.c
	@mkdir -p debug
	LANG=C gcc -c ${CFLAGS} ${DEBUG_CFLAGS} ${GTK_CFLAGS} -o debug/OperatorTable.c.o src/OperatorTable.c

release/OperatorTableUnit.c.o: src/OperatorTableUnit.c
	@mkdir -p release
		LANG=C gcc -c ${CFLAGS} ${RELEASE_CFLAGS} ${GTK_CFLAGS} -o release/OperatorTableUnit.c.o src/OperatorTableUnit.c

debug/OperatorTableUnit.c.o: src/OperatorTableUnit.c
	@mkdir -p debug
	LANG=C gcc -c ${CFLAGS} ${DEBUG_CFLAGS} ${GTK_CFLAGS} -o debug/OperatorTableUnit.c.o src/OperatorTableUnit.c

release/Print.c.o: src/Print.c
	@mkdir -p release
		LANG=C gcc -c ${CFLAGS} ${RELEASE_CFLAGS} ${GTK_CFLAGS} -o release/Print.c.o src/Print.c

debug/Print.c.o: src/Print.c
	@mkdir -p debug
	LANG=C gcc -c ${CFLAGS} ${DEBUG_CFLAGS} ${GTK_CFLAGS} -o debug/Print.c.o src/Print.c

release/PrintFormat.c.o: src/PrintFormat.c
	@mkdir -p release
		LANG=C gcc -c ${CFLAGS} ${RELEASE_CFLAGS} ${GTK_CFLAGS} -o release/PrintFormat.c.o src/PrintFormat.c

debug/PrintFormat.c.o: src/PrintFormat.c
	@mkdir -p debug
	LANG=C gcc -c ${CFLAGS} ${DEBUG_CFLAGS} ${GTK_CFLAGS} -o debug/PrintFormat.c.o src/PrintFormat.c

release/PrintFormatUnit.c.o: src/PrintFormatUnit.c
	@mkdir -p release
		LANG=C gcc -c ${CFLAGS} ${RELEASE_CFLAGS} ${GTK_CFLAGS} -o release/PrintFormatUnit.c.o src/PrintFormatUnit.c

debug/PrintFormatUnit.c.o: src/PrintFormatUnit.c
	@mkdir -p debug
	LANG=C gcc -c ${CFLAGS} ${DEBUG_CFLAGS} ${GTK_CFLAGS} -o debug/PrintFormatUnit.c.o src/PrintFormatUnit.c

release/Quotation.c.o: src/Quotation.c
	@mkdir -p release
		LANG=C gcc -c ${CFLAGS} ${RELEASE_CFLAGS} ${GTK_CFLAGS} -o release/Quotation.c.o src/Quotation.c

debug/Quotation.c.o: src/Quotation.c
	@mkdir -p debug
	LANG=C gcc -c ${CFLAGS} ${DEBUG_CFLAGS} ${GTK_CFLAGS} -o debug/Quotation.c.o src/Quotation.c

