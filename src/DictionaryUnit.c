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
 * $Id: DictionaryUnit.c 247 2010-09-10 10:23:07Z sebtic $
 */

#include <Dictionary.h>
#include <UnitTest.h>

static void test_Dictionary_generic(void)
{
  DictionaryEntry * entry, *oldentry;
  Dictionary * dic = Dictionary_create();

  Dictionary_setNumberEntry(dic, "var1", 10);
  entry = Dictionary_getEntry(dic, "var1");
  ASSERT_NOT_EQUAL(entry, NULL);
  ASSERT_EQUAL_STRING(entry->name, entry->name);
  ASSERT_EQUAL(entry->type, NUMBER_ENTRY);
  ASSERT_EQUAL_DOUBLE(entry->value.numberValue, 10);

  oldentry = entry;
  Dictionary_setNumberEntry(dic, "var1", 1);
  entry = Dictionary_getEntry(dic, "var1");
  ASSERT_NOT_EQUAL(entry, NULL);
  ASSERT_EQUAL_STRING(entry->name, entry->name);
  ASSERT_EQUAL(entry->type, NUMBER_ENTRY);
  ASSERT_EQUAL_DOUBLE(entry->value.numberValue, 1);
  ASSERT_EQUAL(entry, oldentry);

  Dictionary_setStringEntry(dic, "var1", "abc");
  entry = Dictionary_getEntry(dic, "var1");
  ASSERT_NOT_EQUAL(entry, NULL);
  ASSERT_EQUAL_STRING(entry->name, entry->name);
  ASSERT_EQUAL(entry->type, STRING_ENTRY);
  ASSERT_EQUAL_STRING(entry->value.stringValue, "abc");
  ASSERT_EQUAL(entry, oldentry);

  Dictionary_setNumberEntry(dic, "var2", 10);
  entry = Dictionary_getEntry(dic, "var2");
  ASSERT_NOT_EQUAL(entry, NULL);
  ASSERT_EQUAL_STRING(entry->name, entry->name);
  ASSERT_EQUAL(entry->type, NUMBER_ENTRY);
  ASSERT_EQUAL_DOUBLE(entry->value.numberValue, 10);

  Dictionary_destroy(dic);
}

static void test_Dictionary_format(void)
{
  char * result;

  Dictionary * dic = Dictionary_create();
  Dictionary_setNumberEntry(dic, "var1", 10.2);
  Dictionary_setStringEntry(dic, "var2", "abcDef");

  result = Dictionary_format(dic, "%VAR1{precision=0}%");
  ASSERT_EQUAL_STRING(result, "10");
  free(result);

  /* No precision => default C précision : 6 digits */
  result = Dictionary_format(dic, "%VAR1%");
  ASSERT_EQUAL_STRING(result, "10.200000");
  free(result);

  result = Dictionary_format(dic, "%VAR1{precision=0}% %VAR1{precision=0}%");
  ASSERT_EQUAL_STRING(result, "10 10");
  free(result);

  result = Dictionary_format(dic, "%VAR1{precision=2}%");
  ASSERT_EQUAL_STRING(result, "10.20");
  free(result);

  result = Dictionary_format(dic, "%VAR1{precision=2,min=10}%");
  ASSERT_EQUAL_STRING(result, "     10.20");
  free(result);

  result = Dictionary_format(dic, "%VAR2%");
  ASSERT_EQUAL_STRING(result, "abcDef");
  free(result);

  result = Dictionary_format(dic, "%VAR2{max=3}%");
  ASSERT_EQUAL_STRING(result, "abc");
  free(result);

  result = Dictionary_format(dic, "%VAR2{max=10}%");
  ASSERT_EQUAL_STRING(result, "abcDef");
  free(result);

  result = Dictionary_format(dic, "%VAR2{min=8}%");
  ASSERT_EQUAL_STRING(result, "abcDef  ");
  free(result);

  result = Dictionary_format(dic, "%VAR2{case=U}%");
  ASSERT_EQUAL_STRING(result, "ABCDEF");
  free(result);

  result = Dictionary_format(dic, "%VAR2{case=l}%");
  ASSERT_EQUAL_STRING(result, "abcdef");
  free(result);

  result = Dictionary_format(dic, "%VAR2{case=U,max=4}%");
  ASSERT_EQUAL_STRING(result, "ABCD");
  free(result);

  Dictionary_destroy(dic);
}

void test_Dictionary(void)
{
  BEGIN_TESTS(Dictionary)
  {
    RUN_TEST(test_Dictionary_generic);
    RUN_TEST(test_Dictionary_format);
  }
  END_TESTS
}
