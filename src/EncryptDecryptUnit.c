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
 * $Id: EncryptDecryptUnit.c 247 2010-09-10 10:23:07Z sebtic $
 */

#include <EncryptDecrypt.h>
#include <UnitTest.h>

void test_EncryptDecrypt_all(void)
{
  char buf[1024];

  copyStringWithLength(buf, "abcdef", 1024);
  encrypt("AbC", buf);
  ASSERT_EQUAL_STRING(buf, "acedfh");
  decrypt("AbC", buf);
  ASSERT_EQUAL_STRING(buf, "abcdef");

  copyStringWithLength(buf, "abcdef", 1024);
  encrypt("gf", buf);
  ASSERT_EQUAL_STRING(buf, "ggiikk");
  decrypt("gf", buf);
  ASSERT_EQUAL_STRING(buf, "abcdef");

  copyStringWithLength(buf, "abc", 1024);
  encrypt("AbCDEF", buf);
  ASSERT_EQUAL_STRING(buf, "ace");
  decrypt("AbCDEF", buf);
  ASSERT_EQUAL_STRING(buf, "abc");

  copyStringWithLength(buf, "abc", 1024);
  encrypt("gfffffffffffff", buf);
  ASSERT_EQUAL_STRING(buf, "ggh");
  decrypt("gfffffffffffff", buf);
  ASSERT_EQUAL_STRING(buf, "abc");
}

void test_EncryptDecrypt(void)
{
  BEGIN_TESTS(EncryptDecrypt)
  {
    RUN_TEST(test_EncryptDecrypt_all);
  }
  END_TESTS
}
