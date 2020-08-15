#include <CUnit/CUnit.h>
#include "binary_test.h"
#include "src/util/binary.h"
#include "test_utils.h"

void test_remove_padding()
{
    char data[] = {0x00, 0x00, 0x02, 0x01};

    CU_ASSERT_EQUAL(remove_padding(data, 4, 0), 0);
    CU_ASSERT_EQUAL(remove_padding(data, 0, 7), 0);
    CU_ASSERT_EQUAL(remove_padding(data, 4, 7), 257);
    CU_ASSERT_EQUAL(remove_padding(data, 3, 7), 2);
    CU_ASSERT_EQUAL(remove_padding(data, 2, 7), 0);
}

void test_read_int()
{
    const unsigned char _32bitint[4] = {0x01, 0x02, 0x03, 0x04};

    FILE* fp = open_test_file_in_read_write_mode();

    if (!fp) {
        return;
    }

    write_data_to_test_file_and_rewind(_32bitint, 4);

    uint32_t expected;

    // test matching endianness
    read_int(&expected, fp, 1, 0);
    CU_ASSERT_EQUAL(expected, (uint32_t)0x01020304);

    rewind_testfile();

    // test differing endianness
    read_int(&expected, fp, 0, 0);
    CU_ASSERT_EQUAL(expected, (uint32_t)0x04030201);

    close_and_remove_test_file();
}
