#include <CUnit/CUnit.h>
#include "hashmap_test.h"
#include "src/dstructs/hashmap.h"

void test_get_set_and_remove(HashMap* map)
{
    char* a = "a";
    char* b = "b";
    char* c = "c";

    set(map, a, a);

    CU_ASSERT_EQUAL(map->size, 1);
    const void* x = get(map, a);

    CU_ASSERT_PTR_EQUAL(x, a);

    set(map, b, b);

    CU_ASSERT_EQUAL(map->size, 2);
    x = get(map, a);
    CU_ASSERT_PTR_EQUAL(x, a);
    x = get(map, b);
    CU_ASSERT_PTR_EQUAL(x, b);

    x = get(map, c);
    CU_ASSERT_PTR_NULL(x);

    hashmap_remove(map, a);

    CU_ASSERT_EQUAL(map->size, 1);
    x = get(map, a);
    CU_ASSERT_PTR_NULL(x);
    x = get(map, b);
    CU_ASSERT_PTR_EQUAL(x, b);
}

void test_hashmap()
{
    HashMap* map = new_hashmap();

    CU_ASSERT_EQUAL(100, map->n_slots);
    CU_ASSERT_EQUAL(0, map->size);

    test_get_set_and_remove(map);

    delete_hashmap(map);
}

void test_hashmap_with_size()
{
    HashMap* map = new_hashmap_with_size(42);

    CU_ASSERT_EQUAL(42, map->n_slots);
    CU_ASSERT_EQUAL(0, map->size);

    test_get_set_and_remove(map);

    delete_hashmap(map);
}
