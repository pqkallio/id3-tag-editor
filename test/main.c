#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include "binary_test.h"
#include "parser_test.h"

CU_SuiteInfo create_test_suite(char* name, CU_TestInfo* tests)
{
    CU_SuiteInfo info = { name, NULL, NULL, NULL, NULL, tests };

    return info;
}

int main()
{
    CU_ErrorCode err = CU_initialize_registry();

    if (err != CUE_SUCCESS) {
        return err;
    }

    CU_TestInfo util_binary_tests[] = {
        { "remove_padding(char*, size_t, size_t)", test_remove_padding },
        CU_TEST_INFO_NULL,
    };

    CU_TestInfo parser_tests[] = {
        { "ID3 v2 tag", test_id3v2_parser },
        CU_TEST_INFO_NULL,
    };

    CU_SuiteInfo suites[] = {
        create_test_suite("util/binary.c", util_binary_tests),
        create_test_suite("parser/parser.c", parser_tests),
        CU_SUITE_INFO_NULL,
    };

    err = CU_register_suites(suites);

    if (err != CUE_SUCCESS) {
        CU_cleanup_registry();
        return err;
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();

    return CU_get_error();
}
