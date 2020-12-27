#include <CUnit/CUnit.h>
#include "strings_test.h"
#include "src/util/strings.h"
#include "src/mem/mem.h"

void test_string_copy()
{
  char *original = "test";
  char *copy = string_copy(&DEFAULT_MEMMAP, original);

  CU_ASSERT_PTR_NOT_EQUAL(copy, original);
  CU_ASSERT_STRING_EQUAL(original, copy);

  DEFAULT_MEMMAP.free(&DEFAULT_MEMMAP, copy);
}

void test_char_to_hex()
{
  char temp[] = {0, 0, 0};

  char_to_hex(temp, 240);
  CU_ASSERT_STRING_EQUAL(temp, "F0");

  char_to_hex(temp, 225);
  CU_ASSERT_STRING_EQUAL(temp, "E1");

  char_to_hex(temp, 210);
  CU_ASSERT_STRING_EQUAL(temp, "D2");

  char_to_hex(temp, 195);
  CU_ASSERT_STRING_EQUAL(temp, "C3");

  char_to_hex(temp, 180);
  CU_ASSERT_STRING_EQUAL(temp, "B4");

  char_to_hex(temp, 165);
  CU_ASSERT_STRING_EQUAL(temp, "A5");

  char_to_hex(temp, 150);
  CU_ASSERT_STRING_EQUAL(temp, "96");

  char_to_hex(temp, 135);
  CU_ASSERT_STRING_EQUAL(temp, "87");

  char_to_hex(temp, 120);
  CU_ASSERT_STRING_EQUAL(temp, "78");

  char_to_hex(temp, 105);
  CU_ASSERT_STRING_EQUAL(temp, "69");

  char_to_hex(temp, 90);
  CU_ASSERT_STRING_EQUAL(temp, "5A");

  char_to_hex(temp, 75);
  CU_ASSERT_STRING_EQUAL(temp, "4B");

  char_to_hex(temp, 60);
  CU_ASSERT_STRING_EQUAL(temp, "3C");

  char_to_hex(temp, 45);
  CU_ASSERT_STRING_EQUAL(temp, "2D");

  char_to_hex(temp, 30);
  CU_ASSERT_STRING_EQUAL(temp, "1E");

  char_to_hex(temp, 15);
  CU_ASSERT_STRING_EQUAL(temp, "0F");
}
