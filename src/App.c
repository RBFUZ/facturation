/*
 * Copyright 2010 Sébastien Aupetit <sebastien.aupetit@univ-tours.fr>
 *
 * This source code is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * This source code is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this source code. If not, see <http://www.gnu.org/licenses/>.
 *
 * $Id: App.c 247 2010-09-10 10:23:07Z sebtic $
 */

#include <App.h>
#include <MainWindow.h>

#include <MyStringUnit.h>
#include <OperatorTableUnit.h>
#include <EncryptDecryptUnit.h>
#include <CatalogRecordUnit.h>
#include <CatalogDBUnit.h>
#include <CustomerRecordUnit.h>
#include <CustomerDBUnit.h>
#include <DocumentUtilUnit.h>
#include <PrintFormatUnit.h>
#include <DictionaryUnit.h>
#include <DocumentRowListUnit.h>
#include <DocumentUnit.h>
#include <locale.h>
#include <sys/stat.h>
#include <sys/types.h>

void App_run(int * argc, char * *argv[])
{
  Config_init(*argc, *argv);
  Registry_init();

  /* The locale is set to the C locale */
  setlocale(LC_ALL, "C");

  mkdir(BASEPATH, 0777);
  mkdir(BASEPATH "/data", 0777);
  mkdir(BASEPATH "/unittest", 0777);

  if (isSpecified("student-firstname"))
  {
    printf("%s", STUDENT_FIRSTNAME);
    exit(0);
  }

  if (isSpecified("student-lastname"))
  {
    printf("%s", STUDENT_LASTNAME);
    exit(0);
  }

  if (isSpecified("student-years"))
  {
    printf("%s", STUDENT_YEARS);
    exit(0);
  }

  if (isSpecified("student-group"))
  {
    printf("%s", STUDENT_GROUP);
    exit(0);
  }

  setupOverridable();

  if (!isSpecified("silent-tests"))
  {
    printf("Running preliminary unit test... (specify verbose-unittests for details)\n");
  }
  test_MyString();
  test_EncryptDecrypt();
  test_OperatorTable();
  test_CatalogRecord();
  test_CatalogDB();
  test_CustomerRecord();
  test_CustomerDB();
  test_DocumentUtil();
  test_DocumentRowList();
  test_Document();
  test_PrintFormat();
  test_Dictionary();
  if (!isSpecified("silent-tests"))
  {
    printf("Great ! Unit tests passed !\n");
  }

  if (!isSpecified("disable-gui"))
  {
    GtkWidget * window;

    /* Initialise GTK+ passing to it all command line arguments  */
    gtk_init(argc, argv);

    /* Create and display the main window */
    window = MainWindow_create();
    gtk_widget_show(window);

    /* run the GTK+ main loop */
    gtk_main();
  }
}

