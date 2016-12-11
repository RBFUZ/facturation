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
 * $Id: CatalogRecordUnit.c 247 2010-09-10 10:23:07Z sebtic $
 */

#include <CatalogRecord.h>
#include <CatalogRecordEditor.h>
#include <UnitTest.h>
#include <sys/stat.h>
#include <sys/types.h>

static void test_CatalogRecord_accessors(void)
{
  char * str;
  CatalogRecord record;
  CatalogRecord_FieldProperties properties;

  CatalogRecord_init(&record);

  properties = CatalogRecord_getFieldProperties(CATALOGRECORD_CODE_FIELD);
  (*properties.setValue)(&record, "CODE");
  str = (*properties.getValue)(&record);
  ASSERT_EQUAL_STRING(str, "CODE");
  free(str);
  ASSERT_EQUAL_STRING(record.code, "CODE");
  ASSERT(CatalogRecord_isValueValid_code("0984abDe4"));
  ASSERT(!CatalogRecord_isValueValid_code("09.;84abDe4"));

  properties = CatalogRecord_getFieldProperties(CATALOGRECORD_DESIGNATION_FIELD);
  (*properties.setValue)(&record, "DESIGNATION");
  str = (*properties.getValue)(&record);
  ASSERT_EQUAL_STRING(str, "DESIGNATION");
  free(str);
  ASSERT_EQUAL_STRING(record.designation, "DESIGNATION");

  properties = CatalogRecord_getFieldProperties(CATALOGRECORD_UNITY_FIELD);
  (*properties.setValue)(&record, "UNITY");
  str = (*properties.getValue)(&record);
  ASSERT_EQUAL_STRING(str, "UNITY");
  free(str);
  ASSERT_EQUAL_STRING(record.unity, "UNITY");

  ASSERT(CatalogRecord_isValueValid_positiveNumber("12.4"));
  ASSERT(!CatalogRecord_isValueValid_positiveNumber("-12.4"));
  ASSERT(!CatalogRecord_isValueValid_positiveNumber("12de"));

  properties = CatalogRecord_getFieldProperties(CATALOGRECORD_BASEPRICE_FIELD);
  (*properties.setValue)(&record, "10.23");
  str = (*properties.getValue)(&record);
  ASSERT_EQUAL_STRING(str, "10.23");
  free(str);
  ASSERT_EQUAL_DOUBLE(record.basePrice, 10.23);

  properties = CatalogRecord_getFieldProperties(CATALOGRECORD_SELLINGPRICE_FIELD);
  (*properties.setValue)(&record, "9.28");
  str = (*properties.getValue)(&record);
  ASSERT_EQUAL_STRING(str, "9.28");
  free(str);
  ASSERT_EQUAL_DOUBLE(record.sellingPrice, 9.28);

  properties = CatalogRecord_getFieldProperties(CATALOGRECORD_RATEOFVAT_FIELD);
  (*properties.setValue)(&record, "19.60");
  str = (*properties.getValue)(&record);
  ASSERT_EQUAL_STRING(str, "19.60");
  free(str);
  ASSERT_EQUAL_DOUBLE(record.rateOfVAT, 19.6);

  CatalogRecord_finalize(&record);
}

static void setValues(CatalogRecord * record, const char * code, const char * designation, const char * unity, double basePrice, double sellingPrice,
    double rateOfVAT)
{
  CatalogRecord_FieldProperties properties;

  properties = CatalogRecord_getFieldProperties(CATALOGRECORD_CODE_FIELD);
  (*properties.setValue)(record, code);

  properties = CatalogRecord_getFieldProperties(CATALOGRECORD_DESIGNATION_FIELD);
  (*properties.setValue)(record, designation);

  properties = CatalogRecord_getFieldProperties(CATALOGRECORD_UNITY_FIELD);
  (*properties.setValue)(record, unity);

  record->basePrice = basePrice;
  record->sellingPrice = sellingPrice;
  record->rateOfVAT = rateOfVAT;
}

static void testValues(CatalogRecord * record, const char * code, const char * designation, const char * unity, double basePrice, double sellingPrice,
    double rateOfVAT)
{
  ASSERT_EQUAL_STRING(record->code, code);
  ASSERT_EQUAL_STRING(record->designation, designation);
  ASSERT_EQUAL_STRING(record->unity, unity);
  ASSERT_EQUAL_DOUBLE(record->basePrice, basePrice);
  ASSERT_EQUAL_DOUBLE(record->sellingPrice, sellingPrice);
  ASSERT_EQUAL_DOUBLE(record->rateOfVAT, rateOfVAT);
}

static void test_CatalogRecord_readWrite(void)
{
  FILE * file;
  CatalogRecord record;
  long fileSize;

  CatalogRecord_init(&record);

  file = fopen(BASEPATH "/unittest/catalogrecord-unittest.db", "wb");
  setValues(&record, "code1", "designation1", "unity1", 1.1, 2.2, 3.3);
  CatalogRecord_write(&record, file);
  setValues(&record, "code2", "designation2", "unity2", 2.1, 3.2, 4.3);
  CatalogRecord_write(&record, file);
  setValues(&record, "code3", "designation3", "unity3", 3.1, 4.2, 5.3);
  CatalogRecord_write(&record, file);
  fclose(file);

  file = fopen(BASEPATH "/unittest/catalogrecord-unittest.db", "rb");
  CatalogRecord_read(&record, file);
  testValues(&record, "code1", "designation1", "unity1", 1.1, 2.2, 3.3);
  CatalogRecord_read(&record, file);
  testValues(&record, "code2", "designation2", "unity2", 2.1, 3.2, 4.3);
  CatalogRecord_read(&record, file);
  testValues(&record, "code3", "designation3", "unity3", 3.1, 4.2, 5.3);

  /* ftell is bugged on WIN32 platform so skip the test */
#ifndef _WIN32
  fileSize = ftell(file);
  ASSERT_EQUAL(fileSize, CATALOGRECORD_SIZE * 3);
#endif

  fclose(file);

  CatalogRecord_finalize(&record);
}

static void test_CatalogRecord_readWrite2(void)
{
  FILE * file;
  CatalogRecord record;
  long fileSize;
  char codePattern[CATALOGRECORD_CODE_SIZE];
  char designationPattern[CATALOGRECORD_DESIGNATION_SIZE];
  char unityPattern[CATALOGRECORD_UNITY_SIZE];

  memset(codePattern, 'a', CATALOGRECORD_CODE_SIZE);
  memset(designationPattern, 'b', CATALOGRECORD_DESIGNATION_SIZE);
  memset(unityPattern, 'c', CATALOGRECORD_UNITY_SIZE);
  codePattern[CATALOGRECORD_CODE_SIZE-1] = '\0';
  designationPattern[CATALOGRECORD_DESIGNATION_SIZE-1] = '\0';
  unityPattern[CATALOGRECORD_UNITY_SIZE-1] = '\0';


  CatalogRecord_init(&record);

  file = fopen(BASEPATH "/unittest/catalogrecord-unittest.db", "wb");
  setValues(&record, codePattern, designationPattern, unityPattern, 1.1, 2.2, 3.3);
  CatalogRecord_write(&record, file);
  setValues(&record, codePattern, designationPattern, unityPattern, 2.1, 3.2, 4.3);
  CatalogRecord_write(&record, file);
  setValues(&record, codePattern, designationPattern, unityPattern, 3.1, 4.2, 5.3);
  CatalogRecord_write(&record, file);
  fclose(file);

  file = fopen(BASEPATH "/unittest/catalogrecord-unittest.db", "rb");
  CatalogRecord_read(&record, file);
  testValues(&record, codePattern, designationPattern, unityPattern, 1.1, 2.2, 3.3);
  CatalogRecord_read(&record, file);
  testValues(&record, codePattern, designationPattern, unityPattern, 2.1, 3.2, 4.3);
  CatalogRecord_read(&record, file);
  testValues(&record, codePattern, designationPattern, unityPattern, 3.1, 4.2, 5.3);

  /* ftell is bugged on WIN32 platform so skip the test */
#ifndef _WIN32
  fileSize = ftell(file);
  ASSERT_EQUAL(fileSize, CATALOGRECORD_SIZE * 3);
#endif

  fclose(file);

  CatalogRecord_finalize(&record);
}

void test_CatalogRecord(void)
{
  BEGIN_TESTS(CatalogRecord)
  {
    RUN_TEST(test_CatalogRecord_accessors);
    RUN_TEST(test_CatalogRecord_readWrite);
    RUN_TEST(test_CatalogRecord_readWrite2);
  }
  END_TESTS
}

