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
 * $Id: MyStringUnit.c 247 2010-09-10 10:23:07Z sebtic $
 */

#include <MyString.h>
#include <UnitTest.h>

static void test_toLowerChar(void)
{
  ASSERT_EQUAL(toLowerChar('A'), 'a');
  ASSERT_EQUAL(toLowerChar('Z'), 'z');
  ASSERT_EQUAL(toLowerChar('a'), 'a');
  ASSERT_EQUAL(toLowerChar('z'), 'z');
  ASSERT_EQUAL(toLowerChar('!'), '!');
}

static void test_toUpperChar(void)
{
  ASSERT_EQUAL(toUpperChar('A'), 'A');
  ASSERT_EQUAL(toUpperChar('Z'), 'Z');
  ASSERT_EQUAL(toUpperChar('a'), 'A');
  ASSERT_EQUAL(toUpperChar('z'), 'Z');
  ASSERT_EQUAL(toUpperChar('!'), '!');
}

static void test_compareString(void)
{
  ASSERT_NOT_EQUAL(compareString("abc", "def"), 0);
  ASSERT_NOT_EQUAL(compareString("abc", "abcd"), 0);
  ASSERT_NOT_EQUAL(compareString("abcd", "abc"), 0);
  ASSERT_EQUAL(compareString("abc", "abc"), 0);
  ASSERT_EQUAL(compareString("", ""), 0);
  ASSERT_NOT_EQUAL(compareString("", "abcd"), 0);
  ASSERT(compareString("abc", "abcd") < 0);
  ASSERT(compareString("abcd", "abc") > 0);
  ASSERT(compareString("", "abcd") < 0);
  ASSERT(compareString("abc", "") > 0);
}

static void test_icaseCompareString(void)
{
  ASSERT_NOT_EQUAL(icaseCompareString("abc", "def"), 0);
  ASSERT_NOT_EQUAL(icaseCompareString("abc", "abcd"), 0);
  ASSERT_NOT_EQUAL(icaseCompareString("abcd", "abc"), 0);
  ASSERT_EQUAL(icaseCompareString("abc", "abc"), 0);
  ASSERT_EQUAL(icaseCompareString("", ""), 0);
  ASSERT_NOT_EQUAL(icaseCompareString("", "abcd"), 0);
  ASSERT(icaseCompareString("abc", "abcd") < 0);
  ASSERT(icaseCompareString("abcd", "abc") > 0);
  ASSERT(icaseCompareString("", "abcd") < 0);
  ASSERT(icaseCompareString("abc", "") > 0);

  ASSERT_NOT_EQUAL(icaseCompareString("aBc", "Def"), 0);
  ASSERT_NOT_EQUAL(icaseCompareString("ABC", "abcd"), 0);
  ASSERT_NOT_EQUAL(icaseCompareString("abcd", "abc"), 0);
  ASSERT_EQUAL(icaseCompareString("ABC", "abc"), 0);
  ASSERT_EQUAL(icaseCompareString("abc", "ABC"), 0);
  ASSERT_EQUAL(icaseCompareString("", ""), 0);
  ASSERT_NOT_EQUAL(icaseCompareString("", "abcd"), 0);
  ASSERT(icaseCompareString("ABC", "abcd") < 0);
  ASSERT(icaseCompareString("abc", "ABCD") < 0);
  ASSERT(icaseCompareString("abCd", "abc") > 0);
  ASSERT(icaseCompareString("abcd", "abC") > 0);
  ASSERT(icaseCompareString("", "abcd") < 0);
  ASSERT(icaseCompareString("abc", "") > 0);
}

static void test_stringLength(void)
{
  ASSERT_EQUAL(stringLength(""), 0);
  ASSERT_EQUAL(stringLength("abc"), 3);
}

static void test_copyStringWithLength(void)
{
  char buf[10];

  copyStringWithLength(buf, "abcdef", 1);
  ASSERT_EQUAL_STRING(buf, "");

  copyStringWithLength(buf, "abcdef", 3);
  ASSERT_EQUAL_STRING(buf, "ab");

  copyStringWithLength(buf, "abcdef", 7);
  ASSERT_EQUAL_STRING(buf, "abcdef");

  copyStringWithLength(buf, "abcdef", 10);
  ASSERT_EQUAL_STRING(buf, "abcdef");
}

static void test_duplicateString(void)
{
  const char * src = "abc";
  char buf[1024];
  int i, j;
  char * temp = duplicateString(src);

  ASSERT_EQUAL_STRING(src, temp);
  ASSERT(src != temp);
  free(temp);

  for(i = 0; i < 1024; ++i)
  {
    for(j = 0; j < i; ++j)
      buf[j] = (char) ('a' + (j % 26));
    buf[i] = '\0';

    temp = duplicateString(buf);
    ASSERT_EQUAL_STRING(buf, temp);
    free(temp);
  }
}

static void test_icaseStartWith(void)
{
  ASSERT(icaseStartWith("abc", "abcdef"));
  ASSERT(icaseStartWith("ABC", "abcdef"));
  ASSERT(icaseStartWith("abc", "ABCDef"));
  ASSERT(!icaseStartWith("abcg", "abcdef"));
}

static void test_icaseEndWith(void)
{
  ASSERT(icaseEndWith("abc", "defabc"));
  ASSERT(icaseEndWith("ABC", "defabc"));
  ASSERT(icaseEndWith("abc", "DefABC"));
  ASSERT(!icaseEndWith("gabc", "defabc"));
}

static void test_concatenateString(void)
{
  const char* src1 = "abc";
  const char * src2 = "def";
  char * temp, *p;

  temp = concatenateString(src1, src2);

  ASSERT(temp != src1);
  ASSERT(temp != src2);

  p = temp;

  while (*p != '\0' && *src1 != '\0' && *p == *src1)
  {
    p++;
    src1++;
  }
  while (*p != '\0' && *src2 != '\0' && *p == *src2)
  {
    p++;
    src2++;
  }
  ASSERT(*p == '\0' && *src1 == '\0' && *src2 == '\0');
  free(temp);
}

static void test_indexOfChar(void)
{
  const char * src = "aabbccddeeff";

  ASSERT_EQUAL(indexOfChar(src, 'a'), src);
  ASSERT_EQUAL(indexOfChar(src, 'b'), src + 2);
  ASSERT_EQUAL(indexOfChar(src, 'c'), src + 4);
  ASSERT_EQUAL(indexOfChar(src, 'd'), src + 6);
  ASSERT_EQUAL(indexOfChar(src, 'e'), src + 8);
  ASSERT_EQUAL(indexOfChar(src, 'f'), src + 10);
  ASSERT_EQUAL(indexOfChar(src, 'g'), NULL);
  ASSERT_EQUAL(indexOfChar("", 'g'), NULL);
}

static void test_subString(void)
{
  const char * str = "abcdef";
  char * s1 = subString(str, str);
  char * s2 = subString(str, str + strlen(str));
  char * s3 = subString(str + 1, str + 2);

  ASSERT_EQUAL_STRING(s1, "");
  ASSERT_EQUAL_STRING(s2, "abcdef");
  ASSERT_EQUAL_STRING(s3, "b");

  free(s1);
  free(s2);
  free(s3);
}

void test_indexOfString(void)
{
  const char * meule_de_foin = "abcdefabfd";

  ASSERT_EQUAL(indexOfString(meule_de_foin, "abc"), meule_de_foin);
  ASSERT_EQUAL(indexOfString(meule_de_foin, "bcd"), meule_de_foin + 1);
  ASSERT_EQUAL(indexOfString(meule_de_foin, "ab"), meule_de_foin);
  ASSERT_EQUAL(indexOfString(meule_de_foin, "abfd"), meule_de_foin + 6);
  ASSERT_EQUAL(indexOfString(meule_de_foin, "xyz"), NULL);
}

static void test_makeUpperCaseString(void)
{
  char buf[] = "aAbBcCdDz";
  makeUpperCaseString(buf);
  ASSERT_EQUAL_STRING(buf, "AABBCCDDZ");
}

static void test_makeLowerCaseString(void)
{
  char buf[] = "aAbBcCdDz";
  makeLowerCaseString(buf);
  ASSERT_EQUAL_STRING(buf, "aabbccddz");
}

static void test_insertString(void)
{
  const char * src = "abcghi";
  const char * toBeInserted = "def";
  char * temp;

  temp = insertString(src, 3, toBeInserted, 3);
  ASSERT_EQUAL_STRING(temp, "abcdefghi");
  free(temp);

  temp = insertString(src, 3, toBeInserted, 2);
  ASSERT_EQUAL_STRING(temp, "abcdeghi");
  free(temp);

  temp = insertString(src, 0, toBeInserted, 2);
  ASSERT_EQUAL_STRING(temp, "deabcghi");
  free(temp);

  temp = insertString(src, 6, toBeInserted, 2);
  ASSERT_EQUAL_STRING(temp, "abcghide");
  free(temp);
}

void test_MyString(void)
{
  BEGIN_TESTS(MyString)
  {
    RUN_TEST(test_toLowerChar);
    RUN_TEST(test_toUpperChar);
    RUN_TEST(test_compareString);
    RUN_TEST(test_icaseCompareString);
    RUN_TEST(test_stringLength);
    RUN_TEST(test_copyStringWithLength);
    RUN_TEST(test_duplicateString);
    RUN_TEST(test_icaseStartWith);
    RUN_TEST(test_icaseEndWith);
    RUN_TEST(test_concatenateString);
    RUN_TEST(test_indexOfChar);
    RUN_TEST(test_subString);
    RUN_TEST(test_indexOfString);
    RUN_TEST(test_makeUpperCaseString);
    RUN_TEST(test_makeLowerCaseString);
    RUN_TEST(test_insertString);
  }
  END_TESTS
}
