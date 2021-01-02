#include <string.h>
#include "linkedlist.h"
#include "../util/strings.h"

LinkedListItem *new_linked_list_item(const MemMap *memmap, const void *item)
{
    const MemMap *mem = memmap ? memmap : &DEFAULT_MEMMAP;

    LinkedListItem *ll_item = mem->allocate(mem, 1, sizeof(LinkedListItem));

    ll_item->item = item;
    ll_item->memmap = mem;

    return ll_item;
}

const void *delete_linked_list_item(LinkedListItem *item)
{
    if (!item)
        return NULL;

    const MemMap *mem = item->memmap;
    const void *value = item->item;

    mem->free(mem, item);

    return value;
}

void linked_list_append(LinkedList *list, const void *item)
{
    if (!list)
    {
        return;
    }

    LinkedListItem *ll_item = new_linked_list_item(list->memmap, item);

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

const void *linked_list_remove(LinkedList *list, LinkedListItem *item)
{
    if (!list || !item)
    {
        return NULL;
    }

    LinkedListItem *ll_item = list->first;

    while (ll_item != NULL)
    {
        if (ll_item != item)
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

        return delete_linked_list_item(ll_item);
    }

    return NULL;
}

const LinkedListItem *linked_list_find(
    LinkedList *list,
    const void *item,
    int (*comparator)(const LinkedListItem *ll_item, const void *comparee))
{
    if (!list || !item)
    {
        return NULL;
    }

    const LinkedListItem *ll_item = (const LinkedListItem *)list->first;

    while (ll_item)
    {
        if (comparator)
        {
            if (!comparator(ll_item, item))
            {
                return ll_item;
            }
        }
        else if (ll_item->item == item)
        {
            return ll_item;
        }
    }

    return NULL;
}

LinkedList *new_linked_list(const MemMap *memmap)
{
    const MemMap *mem = memmap ? memmap : &DEFAULT_MEMMAP;

    LinkedList *list = mem->allocate(mem, 1, sizeof(LinkedList));

    list->memmap = mem;
    list->append = linked_list_append;
    list->remove = linked_list_remove;
    list->find = linked_list_find;

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
