
#include <CUnit/CUnit.h>
#include "linkedlist_test.h"
#include "src/dstructs/linkedlist.h"
#include "src/domain/tagv2.h"
#include "malloc.h"
#include "src/mem/mem.h"

void test_empty_linkedlist()
{
    LinkedList *ll = new_linked_list(&DEFAULT_MEMMAP);

    CU_ASSERT_EQUAL(ll->size, 0);
    CU_ASSERT_PTR_NULL(ll->first);
    CU_ASSERT_PTR_NULL(ll->last);

    delete_linked_list(ll);
}

void test_append_to_linkedlist()
{
    LinkedList *ll = new_linked_list(&DEFAULT_MEMMAP);
    TagV2Frame *tfs = calloc(2, sizeof(TagV2Frame));

    ll->append(ll, &tfs[0]);

    CU_ASSERT_EQUAL(ll->size, 1);
    CU_ASSERT_EQUAL(ll->first->item, &tfs[0]);
    CU_ASSERT_EQUAL(ll->last->item, &tfs[0]);
    CU_ASSERT_PTR_NULL(ll->first->prev);
    CU_ASSERT_PTR_NULL(ll->first->next);
    CU_ASSERT_PTR_NULL(ll->last->prev);
    CU_ASSERT_PTR_NULL(ll->last->next);

    ll->append(ll, &tfs[1]);

    CU_ASSERT_EQUAL(ll->size, 2);
    CU_ASSERT_EQUAL(ll->first->item, &tfs[0]);
    CU_ASSERT_EQUAL(ll->last->item, &tfs[1]);
    CU_ASSERT_PTR_NULL(ll->first->prev);
    CU_ASSERT_PTR_EQUAL(ll->first->next, ll->last);
    CU_ASSERT_PTR_EQUAL(ll->last->prev, ll->first);
    CU_ASSERT_PTR_NULL(ll->last->next);

    delete_linked_list(ll);
    free(tfs);
}

void test_remove_from_linkedlist()
{
    LinkedList *ll = new_linked_list(&DEFAULT_MEMMAP);

    char a[] = "a";
    char b[] = "b";
    char c[] = "c";
    char d[] = "d";

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
    CU_ASSERT_PTR_EQUAL(ll->first->next->item, &tfs[1]);

    CU_ASSERT_PTR_EQUAL(ll->remove(ll, ll->first->next), &tfs[1]);
    CU_ASSERT_EQUAL(ll->size, 3);
    CU_ASSERT_PTR_EQUAL(ll->first->next->item, &tfs[2]);

    CU_ASSERT_PTR_EQUAL(ll->remove(ll, ll->first), &tfs[0]);
    CU_ASSERT_EQUAL(ll->size, 2);
    CU_ASSERT_PTR_EQUAL(ll->first->item, &tfs[2]);
    CU_ASSERT_PTR_EQUAL(ll->first->next->item, &tfs[3]);
    CU_ASSERT_PTR_EQUAL(ll->last->item, &tfs[3]);

    CU_ASSERT_PTR_EQUAL(ll->remove(ll, ll->last), &tfs[3]);
    CU_ASSERT_EQUAL(ll->size, 1);
    CU_ASSERT_PTR_EQUAL(ll->first->item, &tfs[2]);
    CU_ASSERT_PTR_NULL(ll->first->next);
    CU_ASSERT_PTR_NULL(ll->first->prev);

    CU_ASSERT_PTR_EQUAL(ll->remove(ll, ll->first), &tfs[2]);
    CU_ASSERT_EQUAL(ll->size, 0);
    CU_ASSERT_PTR_NULL(ll->first);
    CU_ASSERT_PTR_NULL(ll->last);

    delete_linked_list(ll);
}

void test_linkedlist_is_null()
{
    LinkedList *ll = new_linked_list(&DEFAULT_MEMMAP);
    void *item = (void *)-1;
    LinkedListItem *lli = DEFAULT_MEMMAP.allocate(&DEFAULT_MEMMAP, 1, sizeof(LinkedListItem));

    ll->append(NULL, item);

    CU_ASSERT_EQUAL(ll->remove(NULL, lli), 0);

    delete_linked_list(NULL);
    delete_linked_list(ll);
    DEFAULT_MEMMAP.free(&DEFAULT_MEMMAP, lli);
}
