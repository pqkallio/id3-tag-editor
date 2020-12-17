#include <CUnit/CUnit.h>
#include "hashmap_test.h"
#include "src/dstructs/hashmap.h"

void test_get_set_and_remove(HashMap *map)
{
    char *a = "a";
    char *b = "b";
    char *c = "c";

    hashmap_set(map, a, a);

    CU_ASSERT_EQUAL(map->size, 1);
    const void *x = hashmap_get(map, a);

    CU_ASSERT_PTR_EQUAL(x, a);

    hashmap_set(map, b, b);

    CU_ASSERT_EQUAL(map->size, 2);
    x = hashmap_get(map, a);
    CU_ASSERT_PTR_EQUAL(x, a);
    x = hashmap_get(map, b);
    CU_ASSERT_PTR_EQUAL(x, b);

    x = hashmap_get(map, c);
    CU_ASSERT_PTR_NULL(x);

    hashmap_remove(map, a);

    CU_ASSERT_EQUAL(map->size, 1);
    x = hashmap_get(map, a);
    CU_ASSERT_PTR_NULL(x);
    x = hashmap_get(map, b);
    CU_ASSERT_PTR_EQUAL(x, b);
}

void test_hashmap()
{
    HashMap *map = new_hashmap();

    CU_ASSERT_EQUAL(100, map->n_slots);
    CU_ASSERT_EQUAL(0, map->size);

    test_get_set_and_remove(map);

    delete_hashmap(map);
}

void test_hashmap_with_size()
{
    HashMap *map = new_hashmap_with_size(42);

    CU_ASSERT_EQUAL(42, map->n_slots);
    CU_ASSERT_EQUAL(0, map->size);

    test_get_set_and_remove(map);

    delete_hashmap(map);
}

void test_hashmap_funcs_with_null_pointers()
{
    HashMap *map = new_hashmap();
    void *value = (void *)-1;

    hashmap_remove(NULL, "key");
    hashmap_remove(map, NULL);
    hashmap_remove(map, "key");

    hashmap_get(NULL, "key");
    hashmap_get(map, NULL);

    hashmap_set(NULL, "key", value);
    hashmap_set(map, NULL, value);
    hashmap_set(map, "key", NULL);

    delete_hashmap(NULL);
}
