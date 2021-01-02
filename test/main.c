#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <stdlib.h>
#include "binary_test.h"
#include "parser_test.h"
#include "linkedlist_test.h"
#include "stack_test.h"
#include "hashmap_test.h"
#include "strings_test.h"
#include "mem_test.h"
#include "hashset_test.h"
#include "unique_id_test.h"

CU_SuiteInfo create_test_suite(char *name, CU_TestInfo *tests)
{
    CU_SuiteInfo info = {name, NULL, NULL, NULL, NULL, tests};

    return info;
}

int main()
{

    CU_ErrorCode err = CU_initialize_registry();

    if (err != CUE_SUCCESS)
    {
        return err;
    }

    CU_TestInfo util_binary_tests[] = {
        {"remove_padding(char*, size_t, size_t)", test_remove_padding},
        {"read_int(uint32_t*, FILE*, int, int)", test_read_int},
        CU_TEST_INFO_NULL,
    };

    CU_TestInfo util_strings_tests[] = {
        {"string_copy(char* str)", test_string_copy},
        {"char_to_hex(char* dst, uint8_t val)", test_char_to_hex},
        CU_TEST_INFO_NULL,
    };

    CU_TestInfo util_unique_id_tests[] = {
        {"0 length", test_unique_id_with_length_0},
        {"2 length", test_unique_id_with_length_2},
        {"4 length", test_unique_id_with_length_4},
        {"5 length", test_unique_id_with_length_5},
        {"6 length", test_unique_id_with_length_6},
        {"32 length", test_unique_id_with_length_32},
        {"254 length", test_unique_id_with_length_254},
        CU_TEST_INFO_NULL,
    };

    CU_TestInfo parser_tests[] = {
        {"ID3 v2 tag", test_id3v2_parser},
        {"Invalid files", test_with_invalid_files},
        CU_TEST_INFO_NULL,
    };

    CU_TestInfo linkedlist_tests[] = {
        {"Empty list", test_empty_linkedlist},
        {"Append to list", test_append_to_linkedlist},
        {"Remove from list", test_remove_from_linkedlist},
        {"Delete a null list", test_linkedlist_is_null},
        CU_TEST_INFO_NULL,
    };

    CU_TestInfo hashmap_tests[] = {
        {"HashMap with default size", test_hashmap},
        {"HashMap with size 42", test_hashmap_with_size},
        {"HashMap tests with null pointers", test_hashmap_funcs_with_null_pointers},
        {"HashMap foreach", test_hashmap_foreach},
        CU_TEST_INFO_NULL,
    };

    CU_TestInfo hashset_tests[] = {
        {"HashSet with default size", test_hashset},
        {"HashSet with size 42", test_hashset_with_size},
        {"HashSet tests with null pointers", test_hashset_funcs_with_null_pointers},
        CU_TEST_INFO_NULL,
    };

    CU_TestInfo stack_tests[] = {
        {"Stack test", test_stack},
        CU_TEST_INFO_NULL,
    };

    CU_TestInfo mem_tests[] = {
        {"MemMap allocation test", test_memmap_allocate},
        {"Very many allocations", test_very_many_allocations},
        CU_TEST_INFO_NULL,
    };

    CU_SuiteInfo suites[] = {
        create_test_suite("util/binary.c", util_binary_tests),
        create_test_suite("util/strings.c", util_strings_tests),
        create_test_suite("util/unique_id.c", util_unique_id_tests),
        create_test_suite("parser/parser.c", parser_tests),
        create_test_suite("dstructs/stack.c", stack_tests),
        create_test_suite("dstructs/linkedlist.c", linkedlist_tests),
        create_test_suite("dstructs/hashmap.c", hashmap_tests),
        create_test_suite("mem/mem.c", mem_tests),
        create_test_suite("dstructs/hashset.c", hashset_tests),
        CU_SUITE_INFO_NULL,
    };

    err = CU_register_suites(suites);

    if (err != CUE_SUCCESS)
    {
        CU_cleanup_registry();
        return err;
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();

    return CU_get_error();
}
