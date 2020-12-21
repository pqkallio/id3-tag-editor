#ifndef _MEM_ALLOCATION_LIST_H
#define _MEM_ALLOCATION_LIST_H

#include <stddef.h>
#include <stdbool.h>

typedef struct _allocation_list_item
{
  const void *item;
  struct _allocation_list_item *prev;
  struct _allocation_list_item *next;
} AllocationListItem;

typedef struct _allocation_list
{
  unsigned long size;
  AllocationListItem *first;
  AllocationListItem *last;

  void (*append)(struct _allocation_list *list, const void *item);
  bool (*remove)(struct _allocation_list *list, const void *item);
} AllocationList;

AllocationList *new_allocation_list();
void delete_allocation_list(AllocationList *list);

#endif /* _MEM_ALLOCATION_LIST_H */
