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
 * $Id: CatalogDBUnit.c 247 2010-09-10 10:23:07Z sebtic $
 */

#include <CatalogDB.h>
#include <UnitTest.h>
#include <CatalogRecord.h>

#include <sys/stat.h>
#include <sys/types.h>

static void test_CatalogDB_openAndCreate(void)
{
  CatalogDB * catalogDB;

  catalogDB = CatalogDB_create(BASEPATH "/unittest/catalogdb-unittest.db");
  ASSERT_NOT_EQUAL(catalogDB, NULL);
  ASSERT_EQUAL(catalogDB->recordCount, 0);
  CatalogDB_close(catalogDB);

  catalogDB = CatalogDB_open(BASEPATH "/unittest/catalogdb-unittest.db");
  ASSERT_NOT_EQUAL(catalogDB, NULL);
  ASSERT_EQUAL(catalogDB->recordCount, 0);
  ASSERT_EQUAL(CatalogDB_getRecordCount(catalogDB), 0);
  CatalogDB_close(catalogDB);

  remove(BASEPATH "/unittest/catalogdb-unittest-doesnotexist.db");
  catalogDB = CatalogDB_open(BASEPATH "/unittest/catalogdb-unittest-doesnotexist.db");
  ASSERT_EQUAL(catalogDB, NULL);
}

static void test_CatalogDB_readAndWrite(void)
{
  CatalogDB * catalogDB;
  CatalogRecord record;
  int i;

  CatalogRecord_init(&record);

  catalogDB = CatalogDB_create(BASEPATH "/unittest/catalogdb-unittest.db");

  for(i = 0; i < 100; ++i)
  {
    record.sellingPrice = i;
    CatalogDB_writeRecord(catalogDB, i, &record);
    ASSERT_EQUAL(catalogDB->recordCount, i + 1);
    ASSERT_EQUAL(CatalogDB_getRecordCount(catalogDB), i + 1);
  }

  for(i = 0; i < 100; ++i)
  {
    CatalogDB_readRecord(catalogDB, i, &record);
    ASSERT_EQUAL_DOUBLE(record.sellingPrice, i);
  }

  CatalogDB_close(catalogDB);

  catalogDB = CatalogDB_open(BASEPATH "/unittest/catalogdb-unittest.db");

  for(i = 0; i < 100; ++i)
  {
    CatalogDB_readRecord(catalogDB, i, &record);
    ASSERT_EQUAL_DOUBLE(record.sellingPrice, i);
  }

  CatalogDB_close(catalogDB);

  CatalogRecord_finalize(&record);
}

static void test_CatalogDB_append(void)
{
  CatalogDB * catalogDB;
  CatalogRecord record;

  CatalogRecord_init(&record);

  catalogDB = CatalogDB_create(BASEPATH "/unittest/catalogdb-unittest.db");
  ASSERT_NOT_EQUAL(catalogDB, NULL);
  ASSERT_EQUAL(catalogDB->recordCount, 0);
  ASSERT_EQUAL(CatalogDB_getRecordCount(catalogDB), 0);

  CatalogDB_appendRecord(catalogDB, &record);
  ASSERT_EQUAL(catalogDB->recordCount, 1);
  ASSERT_EQUAL(CatalogDB_getRecordCount(catalogDB), 1);
  CatalogDB_appendRecord(catalogDB, &record);
  ASSERT_EQUAL(catalogDB->recordCount, 2);
  ASSERT_EQUAL(CatalogDB_getRecordCount(catalogDB), 2);

  CatalogDB_close(catalogDB);

  catalogDB = CatalogDB_open(BASEPATH "/unittest/catalogdb-unittest.db");
  ASSERT_NOT_EQUAL(catalogDB, NULL);
  ASSERT_EQUAL(catalogDB->recordCount, 2);
  ASSERT_EQUAL(CatalogDB_getRecordCount(catalogDB), 2);
  CatalogDB_close(catalogDB);

  catalogDB = CatalogDB_openOrCreate(BASEPATH "/unittest/catalogdb-unittest.db");
  ASSERT_NOT_EQUAL(catalogDB, NULL);
  ASSERT_EQUAL(catalogDB->recordCount, 2);
  ASSERT_EQUAL(CatalogDB_getRecordCount(catalogDB), 2);
  CatalogDB_close(catalogDB);

  remove(BASEPATH "/unittest/catalogdb-unittest-doesnotexist.db");
  catalogDB = CatalogDB_openOrCreate(BASEPATH "/unittest/catalogdb-unittest-doesnotexist.db");
  ASSERT_NOT_EQUAL(catalogDB, NULL);
  ASSERT_EQUAL(catalogDB->recordCount, 0);
  ASSERT_EQUAL(CatalogDB_getRecordCount(catalogDB), 0);
  CatalogDB_close(catalogDB);

  CatalogRecord_finalize(&record);
}

static void test_CatalogDB_insertAndRemove(void)
{
  CatalogDB * catalogDB;
  CatalogRecord record;
  int i;

  CatalogRecord_init(&record);

  catalogDB = CatalogDB_create(BASEPATH "/unittest/catalogdb-unittest.db");

  for(i = 0; i < 100; ++i)
  {
    record.sellingPrice = i;
    CatalogDB_appendRecord(catalogDB, &record);
  }

  for(i = 0; i < 100; i += 2)
  {
    CatalogDB_removeRecord(catalogDB, i);
    record.sellingPrice = 2 * i;
    CatalogDB_insertRecord(catalogDB, i, &record);
  }

  CatalogDB_close(catalogDB);

  catalogDB = CatalogDB_open(BASEPATH "/unittest/catalogdb-unittest.db");

  for(i = 0; i < 100; ++i)
  {
    CatalogDB_readRecord(catalogDB, i, &record);
    if (i % 2 == 0)
      ASSERT_EQUAL_DOUBLE(record.sellingPrice, 2 * i);
    else
      ASSERT_EQUAL_DOUBLE(record.sellingPrice, i);
  }

  CatalogDB_close(catalogDB);

  CatalogRecord_finalize(&record);
}

void test_CatalogDB(void)
{
  BEGIN_TESTS(CatalogDB)
  {
    RUN_TEST(test_CatalogDB_openAndCreate);
    RUN_TEST(test_CatalogDB_readAndWrite);
    RUN_TEST(test_CatalogDB_append);
    RUN_TEST(test_CatalogDB_insertAndRemove);
  }
  END_TESTS
}

