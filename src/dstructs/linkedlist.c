#include <malloc.h>
#include "linkedlist.h"

LinkedListItem* new_linked_list_item(void* item)
{
    LinkedListItem* ll_item = calloc(1, sizeof(LinkedListItem));

    ll_item->item = item;

    return ll_item;
}

void linked_list_append(LinkedList* list, void* item)
{
    LinkedListItem* ll_item = new_linked_list_item(item);

    if (list->first == NULL) {
        list->first = ll_item;
    }

    if (list->last == NULL) {
        list->last = ll_item;

        return;
    }

    ll_item->prev = list->last;
    list->last->next = ll_item;

    list->last = ll_item;

    list->size++;
}

void linked_list_remove(LinkedList* list, void* item)
{
    LinkedListItem* ll_item = list->first;

    while (ll_item != NULL) {
        if (ll_item->item != item) {
            ll_item = ll_item->next;
            continue;
        }

        ll_item->prev->next = ll_item->next;
        ll_item->next->prev = ll_item->prev;
        list->size--;

        free(ll_item);

        break;
    }
}

LinkedList* new_linked_list()
{
    LinkedList* list = calloc(1, sizeof(LinkedList));

    list->append = linked_list_append;
    list->remove = linked_list_remove;

    return list;
}

void delete_linked_list(LinkedList* list)
{
    LinkedListItem* ll_item = list->first;
    LinkedListItem* next = NULL;

    while (ll_item != NULL)
    {
        next = ll_item->next;
        free(ll_item);
        ll_item = next;
    }

    free(list);
}
