#include <CUnit/CUnit.h>
#include "strings_test.h"
#include "src/util/strings.h"

void test_string_copy()
{
  char* original = "test";
  char* copy = string_copy(original);

  CU_ASSERT_PTR_NOT_EQUAL(copy, original);
  CU_ASSERT_STRING_EQUAL(original, copy);

  free(copy);
}
