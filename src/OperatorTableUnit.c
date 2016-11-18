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
 * $Id: OperatorTableUnit.c 247 2010-09-10 10:23:07Z sebtic $
 */

#include <OperatorTable.h>
#include <UnitTest.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <EncryptDecrypt.h>

static void test_OperatorTable_createAndDestroy(void)
{
  OperatorTable * table = OperatorTable_create();
  ASSERT(table != NULL);
  ASSERT_EQUAL(table->recordCount, 0);
  OperatorTable_destroy(table);
}

static void test_OperatorTable_getAndSetOperator(void)
{
  int idx;
  OperatorTable * table = OperatorTable_create();

  idx = OperatorTable_setOperator(table, "moi", "pass");
  ASSERT_EQUAL(idx, 0);
  ASSERT_EQUAL(1, OperatorTable_getRecordCount(table));
  ASSERT_EQUAL_STRING(table->records[0][0], "moi");
  ASSERT_EQUAL_STRING(table->records[0][1], "pass");
  ASSERT_EQUAL_STRING(OperatorTable_getName(table, 0), "moi");
  ASSERT_EQUAL_STRING(OperatorTable_getPassword(table, 0), "pass");

  idx = OperatorTable_setOperator(table, "toi", "tonpass");
  ASSERT_EQUAL(idx, 1);
  ASSERT_EQUAL(2, OperatorTable_getRecordCount(table));
  ASSERT_EQUAL_STRING(table->records[1][0], "toi");
  ASSERT_EQUAL_STRING(table->records[1][1], "tonpass");
  ASSERT_EQUAL_STRING(OperatorTable_getName(table, 1), "toi");
  ASSERT_EQUAL_STRING(OperatorTable_getPassword(table, 1), "tonpass");

  idx = OperatorTable_setOperator(table, "moi", "monpass");
  ASSERT_EQUAL(idx, 0);
  ASSERT_EQUAL(2, OperatorTable_getRecordCount(table));
  ASSERT_EQUAL_STRING(table->records[0][0], "moi");
  ASSERT_EQUAL_STRING(table->records[0][1], "monpass");
  ASSERT_EQUAL_STRING(OperatorTable_getName(table, 0), "moi");
  ASSERT_EQUAL_STRING(OperatorTable_getPassword(table, 0), "monpass");

  OperatorTable_destroy(table);
}

static void test_OperatorTable_findOperator(void)
{

  int idx;
  OperatorTable * table = OperatorTable_create();

  idx = OperatorTable_setOperator(table, "moi", "pass");
  idx = OperatorTable_setOperator(table, "toi", "tonpass");
  idx = OperatorTable_setOperator(table, "moi", "monpass");
  idx = OperatorTable_setOperator(table, "elle", "sonpass");
  idx = OperatorTable_setOperator(table, "lui", "sonpass");
  idx = OperatorTable_setOperator(table, "eux", "leurpass");

  idx = OperatorTable_findOperator(table, "toi");
  ASSERT_EQUAL(idx, 1);
  ASSERT_EQUAL_STRING(OperatorTable_getName(table, idx), "toi");

  idx = OperatorTable_findOperator(table, "moi");
  ASSERT_EQUAL(idx, 0);
  ASSERT_EQUAL_STRING(OperatorTable_getName(table, idx), "moi");

  idx = OperatorTable_findOperator(table, "elle");
  ASSERT_EQUAL(idx, 2);
  ASSERT_EQUAL_STRING(OperatorTable_getName(table, idx), "elle");

  idx = OperatorTable_findOperator(table, "lui");
  ASSERT_EQUAL(idx, 3);
  ASSERT_EQUAL_STRING(OperatorTable_getName(table, idx), "lui");

  idx = OperatorTable_findOperator(table, "eux");
  ASSERT_EQUAL(idx, 4);
  ASSERT_EQUAL_STRING(OperatorTable_getName(table, idx), "eux");

  OperatorTable_destroy(table);
}

static void test_OperatorTable_removeOperator(void)
{

  int idx;
  OperatorTable * table = OperatorTable_create();

  idx = OperatorTable_setOperator(table, "moi", "pass");
  idx = OperatorTable_setOperator(table, "toi", "tonpass");
  idx = OperatorTable_setOperator(table, "moi", "monpass");
  idx = OperatorTable_setOperator(table, "elle", "sonpass");
  idx = OperatorTable_setOperator(table, "lui", "sonpass");
  idx = OperatorTable_setOperator(table, "eux", "leurpass");

  idx = OperatorTable_findOperator(table, "toi");
  ASSERT_EQUAL(idx, 1);
  OperatorTable_removeRecord(table, idx);

  idx = OperatorTable_findOperator(table, "moi");
  ASSERT_EQUAL(idx, 0);
  ASSERT_EQUAL_STRING(OperatorTable_getName(table, idx), "moi");

  idx = OperatorTable_findOperator(table, "elle");
  ASSERT_EQUAL(idx, 1);
  ASSERT_EQUAL_STRING(OperatorTable_getName(table, idx), "elle");

  idx = OperatorTable_findOperator(table, "lui");
  ASSERT_EQUAL(idx, 2);
  ASSERT_EQUAL_STRING(OperatorTable_getName(table, idx), "lui");

  idx = OperatorTable_findOperator(table, "eux");
  ASSERT_EQUAL(idx, 3);
  ASSERT_EQUAL_STRING(OperatorTable_getName(table, idx), "eux");

  ASSERT_EQUAL(OperatorTable_getRecordCount(table), 4);

  OperatorTable_removeRecord(table, OperatorTable_findOperator(table, "moi"));
  ASSERT_EQUAL(OperatorTable_getRecordCount(table), 3);
  ASSERT_EQUAL_STRING(OperatorTable_getName(table, OperatorTable_findOperator(table, "elle")), "elle");
  ASSERT_EQUAL_STRING(OperatorTable_getName(table, OperatorTable_findOperator(table, "lui")), "lui");
  ASSERT_EQUAL_STRING(OperatorTable_getName(table, OperatorTable_findOperator(table, "eux")), "eux");

  OperatorTable_removeRecord(table, OperatorTable_findOperator(table, "eux"));
  ASSERT_EQUAL(OperatorTable_getRecordCount(table), 2);
  ASSERT_EQUAL_STRING(OperatorTable_getName(table, OperatorTable_findOperator(table, "elle")), "elle");
  ASSERT_EQUAL_STRING(OperatorTable_getName(table, OperatorTable_findOperator(table, "lui")), "lui");

  OperatorTable_destroy(table);
}

static void test_OperatorTable_loadAndSave(void)
{
  OperatorTable * table1;
  OperatorTable * table2;

  table1 = OperatorTable_create();

  OperatorTable_setOperator(table1, "moi", "pass");
  OperatorTable_setOperator(table1, "toi", "tonpass");
  OperatorTable_setOperator(table1, "moi", "monpass");
  OperatorTable_setOperator(table1, "elle", "sonpass");
  OperatorTable_setOperator(table1, "lui", "sonpass");
  OperatorTable_setOperator(table1, "eux", "leurpass");

  OperatorTable_saveToFile(table1, BASEPATH "/unittest/operator-unittest.db");
  table2 = OperatorTable_loadFromFile(BASEPATH "/unittest/operator-unittest.db");

  ASSERT_EQUAL(OperatorTable_getRecordCount(table2), 5);
  ASSERT_EQUAL_STRING(OperatorTable_getName(table2, OperatorTable_findOperator(table2, "moi")), "moi");
  ASSERT_EQUAL_STRING(OperatorTable_getPassword(table2, OperatorTable_findOperator(table2, "moi")), "monpass");
  ASSERT_EQUAL_STRING(OperatorTable_getName(table2, OperatorTable_findOperator(table2, "toi")), "toi");
  ASSERT_EQUAL_STRING(OperatorTable_getPassword(table2, OperatorTable_findOperator(table2, "toi")), "tonpass");
  ASSERT_EQUAL_STRING(OperatorTable_getName(table2, OperatorTable_findOperator(table2, "elle")), "elle");
  ASSERT_EQUAL_STRING(OperatorTable_getPassword(table2, OperatorTable_findOperator(table2, "elle")), "sonpass");
  ASSERT_EQUAL_STRING(OperatorTable_getName(table2, OperatorTable_findOperator(table2, "lui")), "lui");
  ASSERT_EQUAL_STRING(OperatorTable_getPassword(table2, OperatorTable_findOperator(table2, "lui")), "sonpass");
  ASSERT_EQUAL_STRING(OperatorTable_getName(table2, OperatorTable_findOperator(table2, "eux")), "eux");
  ASSERT_EQUAL_STRING(OperatorTable_getPassword(table2, OperatorTable_findOperator(table2, "eux")), "leurpass");

  OperatorTable_destroy(table1);
  OperatorTable_destroy(table2);
}

static void test_OperatorTable_loadAndSave2(void)
{
  const char * oldKey;
  char newKey[10] = {0};
  int i;

  OperatorTable * table1;
  OperatorTable * table2;

  table1 = OperatorTable_create();

  OperatorTable_setOperator(table1, "moi", "pass");
  OperatorTable_setOperator(table1, "toi", "tonpass");
  OperatorTable_setOperator(table1, "moi", "monpass");
  OperatorTable_setOperator(table1, "elle", "sonpass");
  OperatorTable_setOperator(table1, "lui", "sonpass");
  OperatorTable_setOperator(table1, "eux", "leurpass");

  /* change the key */
  oldKey = OperatorCryptKey;
  OperatorCryptKey = newKey;

  /* generate a randomized key, no need for a truly uniform distribution then use the mod operator */
  for( i = 0; i < 9; ++i)
    newKey[i] = (char)('A' + rand() % 26);

  OperatorTable_saveToFile(table1, BASEPATH "/unittest/operator-unittest.db");

  /* generate a new randomized key, no need for a truly uniform distribution then use the mod operator */
  for( i = 0; i < 9; ++i)
    newKey[i] = (char)('a' + rand() % 26);

  table2 = OperatorTable_loadFromFile(BASEPATH "/unittest/operator-unittest.db");

  /* the encryption key is different so there must be the correct number of entry but the passwords must not be decryptable */
  ASSERT_EQUAL(OperatorTable_getRecordCount(table2), 5);
  ASSERT_NOT_EQUAL_STRING(OperatorTable_getPassword(table2, 0), "monpass");
  ASSERT_NOT_EQUAL_STRING(OperatorTable_getPassword(table2, 1), "tonpass");
  ASSERT_NOT_EQUAL_STRING(OperatorTable_getPassword(table2, 2), "sonpass");
  ASSERT_NOT_EQUAL_STRING(OperatorTable_getPassword(table2, 3), "sonpass");
  ASSERT_NOT_EQUAL_STRING(OperatorTable_getPassword(table2, 4), "leurpass");

  OperatorTable_destroy(table1);
  OperatorTable_destroy(table2);

  /* restore original key */
  OperatorCryptKey = oldKey;
}

void test_OperatorTable(void)
{
  BEGIN_TESTS(OperatorTable)
  {
    RUN_TEST(test_OperatorTable_createAndDestroy);
    RUN_TEST(test_OperatorTable_getAndSetOperator);
    RUN_TEST(test_OperatorTable_findOperator);
    RUN_TEST(test_OperatorTable_removeOperator);
    RUN_TEST(test_OperatorTable_loadAndSave);
    RUN_TEST(test_OperatorTable_loadAndSave2);
  }
  END_TESTS
}
