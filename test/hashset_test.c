#include <CUnit/CUnit.h>
#include "hashset_test.h"
#include "src/dstructs/hashset.h"
#include "src/mem/mem.h"

void hashset_test_get_set_and_remove(HashSet *set)
{
  char *a = "a";
  char *b = "b";
  char *c = "c";

  CU_ASSERT_EQUAL(set->add(set, a), HSE_SUCCESS);

  CU_ASSERT_EQUAL(set->size, 1);

  CU_ASSERT_PTR_EQUAL(set->get(set, a), a);

  CU_ASSERT_EQUAL(set->add(set, b), HSE_SUCCESS);
  CU_ASSERT_EQUAL(set->add(set, b), HSE_ALREADY_EXISTS);

  CU_ASSERT_EQUAL(set->size, 2);
  CU_ASSERT_PTR_EQUAL(set->get(set, a), a);
  CU_ASSERT_PTR_EQUAL(set->get(set, b), b);

  CU_ASSERT_PTR_NULL(set->get(set, c));

  CU_ASSERT_EQUAL(set->remove(set, a), HSE_SUCCESS);
  CU_ASSERT_EQUAL(set->remove(set, a), HSE_NOT_FOUND);
  CU_ASSERT_EQUAL(set->remove(set, c), HSE_NOT_FOUND);

  CU_ASSERT_EQUAL(set->size, 1);
  CU_ASSERT_PTR_NULL(set->get(set, a));
  CU_ASSERT_PTR_EQUAL(set->get(set, b), b);
}

void test_hashset()
{
  HashSet *set = new_hashset(&DEFAULT_MEMMAP, NULL, NULL);

  CU_ASSERT_EQUAL(DEFAULT_N_SLOTS, set->n_slots);
  CU_ASSERT_EQUAL(0, set->size);

  hashset_test_get_set_and_remove(set);

  delete_hashset(set);
}

void test_hashset_with_size()
{
  HashSet *set = new_hashset_with_size(&DEFAULT_MEMMAP, NULL, NULL, 42);

  CU_ASSERT_EQUAL(42, set->n_slots);
  CU_ASSERT_EQUAL(0, set->size);

  hashset_test_get_set_and_remove(set);

  delete_hashset(set);
}

void test_hashset_funcs_with_null_pointers()
{
  HashSet *set = new_hashset(&DEFAULT_MEMMAP, NULL, NULL);
  void *value = (void *)-1;

  CU_ASSERT_EQUAL(set->remove(set, NULL), HSE_SET_OR_ITEM_NOT_PROVIDED);
  CU_ASSERT_EQUAL(set->remove(NULL, value), HSE_SET_OR_ITEM_NOT_PROVIDED);

  CU_ASSERT_PTR_NULL(set->get(set, NULL));
  CU_ASSERT_PTR_NULL(set->get(NULL, value));

  CU_ASSERT_EQUAL(set->add(set, NULL), HSE_SET_OR_ITEM_NOT_PROVIDED);
  CU_ASSERT_EQUAL(set->add(NULL, value), HSE_SET_OR_ITEM_NOT_PROVIDED);

  delete_hashset(NULL);
  delete_hashset(set);
}
