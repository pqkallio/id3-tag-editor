#include <string.h>
#include "linkedlist.h"
#include "../util/strings.h"

LinkedListItem *new_linked_list_item(const MemMap *memmap, const char *key, const void *item)
{
    MemMap *mem = memmap ? memmap : &DEFAULT_MEMMAP;

    LinkedListItem *ll_item = mem->allocate(mem, 1, sizeof(LinkedListItem));
    char *item_key = string_copy(mem, key);

    ll_item->item = item;
    ll_item->key = item_key;

    return ll_item;
}

void delete_linked_list_item(LinkedListItem *item)
{
    if (!item)
        return;

    const MemMap *mem = item->memmap;

    mem->free(mem, item->key);
    mem->free(mem, item);
}

void linked_list_append(LinkedList *list, const char *key, const void *item)
{
    if (!list)
    {
        return;
    }

    LinkedListItem *ll_item = new_linked_list_item(list->memmap, key, item);

    list->size++;

    if (list->first == NULL)
    {
        list->first = ll_item;
    }

    if (list->last == NULL)
    {
        list->last = ll_item;

        return;
    }

    ll_item->prev = list->last;
    list->last->next = ll_item;
    list->last = ll_item;

    ll_item = list->first;
}

bool linked_list_remove(LinkedList *list, const char *key)
{
    if (!list)
    {
        return false;
    }

    LinkedListItem *ll_item = list->first;

    while (ll_item != NULL)
    {
        if (strcmp(ll_item->key, key))
        {
            ll_item = ll_item->next;
            continue;
        }

        list->size--;

        if (ll_item->prev)
        {
            ll_item->prev->next = ll_item->next;
        }

        if (ll_item->next)
        {
            ll_item->next->prev = ll_item->prev;
        }

        if (ll_item == list->first)
        {
            list->first = ll_item->next;
        }

        if (ll_item == list->last)
        {
            list->last = ll_item->prev;
        }

        delete_linked_list_item(ll_item);

        return true;
    }

    return false;
}

LinkedList *new_linked_list(const MemMap *memmap)
{
    MemMap *mem = memmap ? memmap : &DEFAULT_MEMMAP;

    LinkedList *list = mem->allocate(mem, 1, sizeof(LinkedList));

    list->memmap = mem;
    list->append = linked_list_append;
    list->remove = linked_list_remove;

    return list;
}

void delete_linked_list(LinkedList *list)
{
    if (!list)
    {
        return;
    }

    LinkedListItem *ll_item = list->first;
    LinkedListItem *next = NULL;

    while (ll_item != NULL)
    {
        next = ll_item->next;
        delete_linked_list_item(ll_item);
        ll_item = next;
    }

    list->memmap->free(list->memmap, list);
}
