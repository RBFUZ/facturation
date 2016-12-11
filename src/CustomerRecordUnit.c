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
 * $Id: CustomerRecordUnit.c 247 2010-09-10 10:23:07Z sebtic $
 */

#include <CustomerRecord.h>
#include <CustomerRecordEditor.h>
#include <UnitTest.h>
#include <sys/stat.h>
#include <sys/types.h>

static void test_CustomerRecord_accessors(void)
{
  char * str;
  CustomerRecord record;
  CustomerRecord_FieldProperties properties;

  CustomerRecord_init(&record);

  properties = CustomerRecord_getFieldProperties(CUSTOMERRECORD_NAME_FIELD);
  (*properties.setValue)(&record, "NAME");
  str = (*properties.getValue)(&record);
  ASSERT_EQUAL_STRING(str, "NAME");
  free(str);
  ASSERT_EQUAL_STRING(record.name, "NAME");

  properties = CustomerRecord_getFieldProperties(CUSTOMERRECORD_ADDRESS_FIELD);
  (*properties.setValue)(&record, "ADDRESS");
  str = (*properties.getValue)(&record);
  ASSERT_EQUAL_STRING(str, "ADDRESS");
  free(str);
  ASSERT_EQUAL_STRING(record.address, "ADDRESS");

  properties = CustomerRecord_getFieldProperties(CUSTOMERRECORD_POSTALCODE_FIELD);
  (*properties.setValue)(&record, "POSTAL");
  str = (*properties.getValue)(&record);
  ASSERT_EQUAL_STRING(str, "POSTAL");
  free(str);
  ASSERT_EQUAL_STRING(record.postalCode, "POSTAL");

  properties = CustomerRecord_getFieldProperties(CUSTOMERRECORD_TOWN_FIELD);
  (*properties.setValue)(&record, "TOWN");
  str = (*properties.getValue)(&record);
  ASSERT_EQUAL_STRING(str, "TOWN");
  free(str);
  ASSERT_EQUAL_STRING(record.town, "TOWN");

  CustomerRecord_finalize(&record);
}

static void setValues(CustomerRecord * record, const char * name, const char * address, const char * postalCode, const char * town)
{
  CustomerRecord_FieldProperties properties;

  properties = CustomerRecord_getFieldProperties(CUSTOMERRECORD_NAME_FIELD);
  (*properties.setValue)(record, name);

  properties = CustomerRecord_getFieldProperties(CUSTOMERRECORD_ADDRESS_FIELD);
  (*properties.setValue)(record, address);

  properties = CustomerRecord_getFieldProperties(CUSTOMERRECORD_POSTALCODE_FIELD);
  (*properties.setValue)(record, postalCode);

  properties = CustomerRecord_getFieldProperties(CUSTOMERRECORD_TOWN_FIELD);
  (*properties.setValue)(record, town);
}

static void testValues(CustomerRecord * record, const char * name, const char * address, const char * postalCode, const char * town)
{
  ASSERT_EQUAL_STRING(record->name, name);
  ASSERT_EQUAL_STRING(record->address, address);
  ASSERT_EQUAL_STRING(record->postalCode, postalCode);
  ASSERT_EQUAL_STRING(record->town, town);
}

static void test_CustomerRecord_readWrite(void)
{
  FILE * file;
  CustomerRecord record;

  CustomerRecord_init(&record);

  file = fopen(BASEPATH "/unittest/catalogrecord-unittest.db", "w+b");
  setValues(&record, "name1", "address1", "postalcode1", "town1");
  CustomerRecord_write(&record, file);
  setValues(&record, "name2", "address2", "postalcode2", "town2");
  CustomerRecord_write(&record, file);
  setValues(&record, "name3", "address3", "postalcode3", "town3");
  CustomerRecord_write(&record, file);

  fseek(file, 0, SEEK_SET);
  CustomerRecord_read(&record, file);
  testValues(&record, "name1", "address1", "postalcode1", "town1");
  CustomerRecord_read(&record, file);
  testValues(&record, "name2", "address2", "postalcode2", "town2");
  CustomerRecord_read(&record, file);
  testValues(&record, "name3", "address3", "postalcode3", "town3");

  ASSERT_EQUAL(ftell(file), CUSTOMERRECORD_SIZE * 3);

  fclose(file);

  CustomerRecord_finalize(&record);
}

static void test_CustomerRecord_readWrite2(void)
{
  FILE * file;
  char namePattern[CUSTOMERRECORD_NAME_SIZE];
  char addressPattern[CUSTOMERRECORD_ADDRESS_SIZE];
  char postalcodePattern[CUSTOMERRECORD_POSTALCODE_SIZE];
  char townPattern[CUSTOMERRECORD_TOWN_SIZE];

  memset(namePattern, 'a', CUSTOMERRECORD_NAME_SIZE);
  memset(addressPattern, 'b', CUSTOMERRECORD_ADDRESS_SIZE);
  memset(postalcodePattern, 'c', CUSTOMERRECORD_POSTALCODE_SIZE);
  memset(townPattern, 'd', CUSTOMERRECORD_TOWN_SIZE);
  namePattern[CUSTOMERRECORD_NAME_SIZE-1] = '\0';
  addressPattern[CUSTOMERRECORD_ADDRESS_SIZE-1] = '\0';
  postalcodePattern[CUSTOMERRECORD_POSTALCODE_SIZE-1] = '\0';
  townPattern[CUSTOMERRECORD_TOWN_SIZE-1] = '\0';

  CustomerRecord record;

  CustomerRecord_init(&record);

  file = fopen(BASEPATH "/unittest/catalogrecord-unittest.db", "w+b");
  setValues(&record, namePattern, addressPattern, postalcodePattern, townPattern);
  CustomerRecord_write(&record, file);
  CustomerRecord_write(&record, file);
  CustomerRecord_write(&record, file);

  fseek(file, 0, SEEK_SET);
  CustomerRecord_read(&record, file);
  testValues(&record, namePattern, addressPattern, postalcodePattern, townPattern);
  CustomerRecord_read(&record, file);
  testValues(&record, namePattern, addressPattern, postalcodePattern, townPattern);
  CustomerRecord_read(&record, file);
  testValues(&record, namePattern, addressPattern, postalcodePattern, townPattern);

  ASSERT_EQUAL(ftell(file), CUSTOMERRECORD_SIZE * 3);

  fclose(file);

  CustomerRecord_finalize(&record);
}

void test_CustomerRecord(void)
{
  BEGIN_TESTS(CustomerRecord)
  {
    RUN_TEST(test_CustomerRecord_accessors);
    RUN_TEST(test_CustomerRecord_readWrite);
    RUN_TEST(test_CustomerRecord_readWrite2);
  }
  END_TESTS
}

