#include <CUnit/CUnit.h>
#include "binary_test.h"
#include "src/util/binary.h"

void test_remove_padding()
{
    char data[] = {0x00, 0x00, 0x02, 0x01};

    CU_ASSERT_EQUAL(remove_padding(data, 4, 0), 0);
    CU_ASSERT_EQUAL(remove_padding(data, 0, 7), 0);
    CU_ASSERT_EQUAL(remove_padding(data, 4, 7), 257);
    CU_ASSERT_EQUAL(remove_padding(data, 3, 7), 2);
    CU_ASSERT_EQUAL(remove_padding(data, 2, 7), 0);
}
