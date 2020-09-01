
#include <CUnit/CUnit.h>
#include "linkedlist_test.h"
#include "src/dstructs/linkedlist.h"
#include "src/domain/tagv2.h"

void test_empty_linkedlist()
{
  LinkedList* ll = new_linked_list();

  CU_ASSERT_EQUAL(ll->size, 0);
  CU_ASSERT_PTR_NULL(ll->first);
  CU_ASSERT_PTR_NULL(ll->last);

  delete_linked_list(ll);
}

void test_append_to_linkedlist()
{
  LinkedList* ll = new_linked_list();
  TagV2Frame tfs[2];

  ll->append(ll, &tfs[0]);

  CU_ASSERT_EQUAL(ll->size, 1);
  CU_ASSERT_EQUAL(ll->first->frame, &tfs[0]);
  CU_ASSERT_EQUAL(ll->last->frame, &tfs[0]);
  CU_ASSERT_PTR_NULL(ll->first->prev);
  CU_ASSERT_PTR_NULL(ll->first->next);
  CU_ASSERT_PTR_NULL(ll->last->prev);
  CU_ASSERT_PTR_NULL(ll->last->next);

  ll->append(ll, &tfs[1]);

  CU_ASSERT_EQUAL(ll->size, 2);
  CU_ASSERT_EQUAL(ll->first->frame, &tfs[0]);
  CU_ASSERT_EQUAL(ll->last->frame, &tfs[1]);
  CU_ASSERT_PTR_NULL(ll->first->prev);
  CU_ASSERT_PTR_EQUAL(ll->first->next, ll->last);
  CU_ASSERT_PTR_EQUAL(ll->last->prev, ll->first);
  CU_ASSERT_PTR_NULL(ll->last->next);

  delete_linked_list(ll);
}

void test_remove_from_linkedlist()
{
  LinkedList* ll = new_linked_list();

  char a[] = "a";
  char b[] = "b";
  char c[] = "c";
  char d[] = "d";
  char e[] = "e";

  TagV2Frame tfs[5] = {
    {.header.id = a},
    {.header.id = b},
    {.header.id = c},
    {.header.id = d},
  };

  ll->append(ll, &tfs[0]);
  ll->append(ll, &tfs[1]);
  ll->append(ll, &tfs[2]);
  ll->append(ll, &tfs[3]);

  CU_ASSERT_EQUAL(ll->size, 4);
  CU_ASSERT_PTR_EQUAL(ll->first->next->frame, &tfs[1]);

  ll->remove(ll, b);

  CU_ASSERT_EQUAL(ll->size, 3);
  CU_ASSERT_PTR_EQUAL(ll->first->next->frame, &tfs[2]);

  ll->remove(ll, e);

  CU_ASSERT_EQUAL(ll->size, 3);

  ll->remove(ll, a);

  CU_ASSERT_EQUAL(ll->size, 2);
  CU_ASSERT_PTR_EQUAL(ll->first->frame, &tfs[2]);
  CU_ASSERT_PTR_EQUAL(ll->first->next->frame, &tfs[3]);
  CU_ASSERT_PTR_EQUAL(ll->last->frame, &tfs[3]);

  delete_linked_list(ll);
}
