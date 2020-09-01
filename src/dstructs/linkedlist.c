#include <malloc.h>
#include <string.h>
#include "linkedlist.h"

LinkedListItem* new_linked_list_item(TagV2Frame* frame)
{
    LinkedListItem* ll_item = calloc(1, sizeof(LinkedListItem));

    ll_item->frame = frame;

    return ll_item;
}

void linked_list_append(LinkedList* list, TagV2Frame* frame)
{
    LinkedListItem* ll_item = new_linked_list_item(frame);

    list->size++;

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
}

void linked_list_remove(LinkedList* list, char* frame_id)
{
    LinkedListItem* ll_item = list->first;

    while (ll_item != NULL) {
        if (strcmp(ll_item->frame->header.id, frame_id)) {
            ll_item = ll_item->next;
            continue;
        }

        ll_item->prev->next = ll_item->next;
        ll_item->next->prev = ll_item->prev;
        list->size--;

        if (ll_item == list->first) {
            list->first = ll_item->next;
        }

        if (ll_item == list->last) {
            list->last = ll_item->prev;
        }

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
