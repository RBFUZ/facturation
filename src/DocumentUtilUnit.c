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
 * $Id: DocumentUtilUnit.c 247 2010-09-10 10:23:07Z sebtic $
 */

#include <DocumentUtil.h>
#include <UnitTest.h>
#include <sys/stat.h>
#include <sys/types.h>

static void test_computeDocumentNumber(void)
{
  char * result = computeDocumentNumber(123456789);
  ASSERT_EQUAL_STRING(result, "21I3V9");
  free(result);
}

static void test_formatDate(void)
{
  char * result = formatDate(1, 9, 2010);
  ASSERT_EQUAL_STRING(result, "01/09/2010");
  free(result);
}

static void test_readAndWriteString(void)
{
  FILE * file;
  char * result;

  file = fopen(BASEPATH "/unittest/readwritestring-unittest.db", "w+b");

  writeString("abc", file);
  writeString("abc\ndef", file);
  writeString("abc\n\tdef\n", file);

  fseek(file, 0, SEEK_SET);

  result = readString(file);
  ASSERT_EQUAL_STRING(result, "abc");
  free(result);

  result = readString(file);
  ASSERT_EQUAL_STRING(result, "abc\ndef");
  free(result);

  result = readString(file);
  ASSERT_EQUAL_STRING(result, "abc\n\tdef\n");
  free(result);

  fclose(file);
}

void test_DocumentUtil(void)
{
  BEGIN_TESTS(DocumentUtil)
  {
    RUN_TEST(test_computeDocumentNumber);
    RUN_TEST(test_formatDate);
    RUN_TEST(test_readAndWriteString);
  }
  END_TESTS
}
