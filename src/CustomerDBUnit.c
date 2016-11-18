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
 * $Id: CustomerDBUnit.c 247 2010-09-10 10:23:07Z sebtic $
 */

#include <CustomerDB.h>
#include <UnitTest.h>
#include <CustomerRecord.h>
#include <CustomerRecordEditor.h>

static void test_CustomerDB_openAndCreate(void)
{
  CustomerDB * customerDB;

  customerDB = CustomerDB_create(BASEPATH "/unittest/customerdb-unittest.db");
  ASSERT_NOT_EQUAL(customerDB, NULL);
  ASSERT_EQUAL(customerDB->recordCount, 0);
  CustomerDB_close(customerDB);

  customerDB = CustomerDB_open(BASEPATH "/unittest/customerdb-unittest.db");
  ASSERT_NOT_EQUAL(customerDB, NULL);
  ASSERT_EQUAL(customerDB->recordCount, 0);
  ASSERT_EQUAL(CustomerDB_getRecordCount(customerDB), 0);
  CustomerDB_close(customerDB);

  remove(BASEPATH "/unittest/customerdb-unittest-doesnotexist.db");
  customerDB = CustomerDB_open(BASEPATH "/unittest/customerdb-unittest-doesnotexist.db");
  ASSERT_EQUAL(customerDB, NULL);
}

static void test_CustomerDB_readAndWrite(void)
{
  CustomerDB * customerDB;
  CustomerRecord record;
  int i;
  CustomerRecord_FieldProperties properties;

  properties = CustomerRecord_getFieldProperties(CUSTOMERRECORD_NAME_FIELD);

  CustomerRecord_init(&record);

  customerDB = CustomerDB_create(BASEPATH "/unittest/customerdb-unittest.db");

  for(i = 0; i < 100; ++i)
  {
    char buf[1024];
    snprintf(buf, 1024, "%d", i);
    (*properties.setValue)(&record, buf);
    CustomerDB_writeRecord(customerDB, i, &record);
    ASSERT_EQUAL(customerDB->recordCount, i + 1);
    ASSERT_EQUAL(CustomerDB_getRecordCount(customerDB), i + 1);
  }

  for(i = 0; i < 100; ++i)
  {
    char buf[1024];
    snprintf(buf, 1024, "%d", i);
    CustomerDB_readRecord(customerDB, i, &record);
    ASSERT_EQUAL_STRING(record.name, buf);
  }

  CustomerDB_close(customerDB);

  customerDB = CustomerDB_open(BASEPATH "/unittest/customerdb-unittest.db");

  for(i = 0; i < 100; ++i)
  {
    char buf[1024];
    snprintf(buf, 1024, "%d", i);
    CustomerDB_readRecord(customerDB, i, &record);
    ASSERT_EQUAL_STRING(record.name, buf);
  }

  CustomerDB_close(customerDB);

  CustomerRecord_finalize(&record);
}

static void test_CustomerDB_append(void)
{
  CustomerDB * customerDB;
  CustomerRecord record;

  CustomerRecord_init(&record);

  customerDB = CustomerDB_create(BASEPATH "/unittest/customerdb-unittest.db");
  ASSERT_NOT_EQUAL(customerDB, NULL);
  ASSERT_EQUAL(customerDB->recordCount, 0);
  ASSERT_EQUAL(CustomerDB_getRecordCount(customerDB), 0);

  CustomerDB_appendRecord(customerDB, &record);
  ASSERT_EQUAL(customerDB->recordCount, 1);
  ASSERT_EQUAL(CustomerDB_getRecordCount(customerDB), 1);
  CustomerDB_appendRecord(customerDB, &record);
  ASSERT_EQUAL(customerDB->recordCount, 2);
  ASSERT_EQUAL(CustomerDB_getRecordCount(customerDB), 2);

  CustomerDB_close(customerDB);

  customerDB = CustomerDB_open(BASEPATH "/unittest/customerdb-unittest.db");
  ASSERT_NOT_EQUAL(customerDB, NULL);
  ASSERT_EQUAL(customerDB->recordCount, 2);
  ASSERT_EQUAL(CustomerDB_getRecordCount(customerDB), 2);
  CustomerDB_close(customerDB);

  customerDB = CustomerDB_openOrCreate(BASEPATH "/unittest/customerdb-unittest.db");
  ASSERT_NOT_EQUAL(customerDB, NULL);
  ASSERT_EQUAL(customerDB->recordCount, 2);
  ASSERT_EQUAL(CustomerDB_getRecordCount(customerDB), 2);
  CustomerDB_close(customerDB);

  remove(BASEPATH "/unittest/catalogdb-unittest-doesnotexist.db");
  customerDB = CustomerDB_openOrCreate(BASEPATH "/unittest/customerdb-unittest-doesnotexist.db");
  ASSERT_NOT_EQUAL(customerDB, NULL);
  ASSERT_EQUAL(customerDB->recordCount, 0);
  ASSERT_EQUAL(CustomerDB_getRecordCount(customerDB), 0);
  CustomerDB_close(customerDB);

  CustomerRecord_finalize(&record);
}

static void test_CustomerDB_insertAndRemove(void)
{
  CustomerDB * customerDB;
  CustomerRecord record;
  int i;
  CustomerRecord_FieldProperties properties;

  properties = CustomerRecord_getFieldProperties(CUSTOMERRECORD_NAME_FIELD);

  CustomerRecord_init(&record);

  customerDB = CustomerDB_create(BASEPATH "/unittest/customerdb-unittest.db");

  for(i = 0; i < 100; ++i)
  {
    char buf[1024];
    snprintf(buf, 1024, "%d", i);
    (*properties.setValue)(&record, buf);
    CustomerDB_appendRecord(customerDB, &record);
  }

  for(i = 0; i < 100; i += 2)
  {
    char buf[1024];

    CustomerDB_removeRecord(customerDB, i);

    snprintf(buf, 1024, "%d", 2 * i);
    (*properties.setValue)(&record, buf);
    CustomerDB_insertRecord(customerDB, i, &record);
  }

  CustomerDB_close(customerDB);

  customerDB = CustomerDB_open(BASEPATH "/unittest/customerdb-unittest.db");

  for(i = 0; i < 100; ++i)
  {
    char buf[1024];
    if (i % 2 == 0)
      snprintf(buf, 1024, "%d", 2 * i);
    else
      snprintf(buf, 1024, "%d", i);

    CustomerDB_readRecord(customerDB, i, &record);
    ASSERT_EQUAL_STRING(record.name, buf);
  }

  CustomerDB_close(customerDB);

  CustomerRecord_finalize(&record);
}

void test_CustomerDB(void)
{
  BEGIN_TESTS(CustomerDB)
  {
    RUN_TEST(test_CustomerDB_openAndCreate);
    RUN_TEST(test_CustomerDB_readAndWrite);
    RUN_TEST(test_CustomerDB_append);
    RUN_TEST(test_CustomerDB_insertAndRemove);
  }
  END_TESTS
}

