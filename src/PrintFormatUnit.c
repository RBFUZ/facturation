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
 * $Id: PrintFormatUnit.c 247 2010-09-10 10:23:07Z sebtic $
 */

#include <PrintFormat.h>
#include <UnitTest.h>

void test_PrintFormat_all(void)
{
  const char * name = "Format de devis n°1";
  const char * header = "l'entete\n    sur\n        plusieurs\n            lignes";
  const char * row = "une ligne";
  const char * footer = "le pied\nde\npage";
  PrintFormat printFormat;

  FILE * input = fopen(BASEPATH "/unittest/unittest-printformat-unittest.txt", "wt");
  fprintf(input, ".NAME %s\n.HEADER\n%s\n.ROW\n%s\n.FOOTER\n%s\n.END", name, header, row, footer);
  fclose(input);

  PrintFormat_init(&printFormat);
  PrintFormat_loadFromFile(&printFormat, BASEPATH "/unittest/unittest-printformat-unittest.txt");
  ASSERT_EQUAL_STRING(name, printFormat.name);
  ASSERT_EQUAL_STRING(header, printFormat.header);
  ASSERT_EQUAL_STRING(row, printFormat.row);
  ASSERT_EQUAL_STRING(footer, printFormat.footer);

  PrintFormat_finalize(&printFormat);
}

void test_PrintFormat(void)
{
  BEGIN_TESTS(PrintFormat)
  {
    RUN_TEST(test_PrintFormat_all);
  }
  END_TESTS
}

