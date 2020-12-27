#ifndef _DSTRUCTS_LINKEDLIST_H
#define _DSTRUCTS_LINKEDLIST_H

#include "../mem/mem.h"

typedef struct _linked_list_item
{
    const MemMap *memmap;

    const void *item;
    struct _linked_list_item *prev;
    struct _linked_list_item *next;
} LinkedListItem;

typedef struct _linked_list
{
    const MemMap *memmap;

    unsigned long size;
    struct _linked_list_item *first;
    struct _linked_list_item *last;

    void (*append)(struct _linked_list *list, const void *item);
    const LinkedListItem *(*find)(
        struct _linked_list *list,
        const void *item,
        int (*comparator)(const LinkedListItem *ll_item, const void *comparee));
    const void *(*remove)(struct _linked_list *list, LinkedListItem *item);
} LinkedList;

LinkedList *new_linked_list(const MemMap *memmap);
void delete_linked_list(LinkedList *list);

#endif /* _DSTRUCTS_LINKEDLIST_H */
