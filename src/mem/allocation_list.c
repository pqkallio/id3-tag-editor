#include <malloc.h>
#include <string.h>
#include "allocation_list.h"

AllocationListItem *new_allocation_list_item(const void *item)
{
  AllocationListItem *al_item = calloc(1, sizeof(AllocationListItem));
  al_item->item = item;

  return al_item;
}

void delete_allocation_list_item(AllocationListItem *item)
{
  if (item->item)
    free((void *)item->item);

  if (item)
    free(item);
}

void allocation_list_append(AllocationList *list, const void *item)
{
  if (!list)
  {
    return;
  }

  AllocationListItem *al_item = new_allocation_list_item(item);

  list->size++;

  if (list->first == NULL)
  {
    list->first = al_item;
  }

  if (list->last == NULL)
  {
    list->last = al_item;

    return;
  }

  al_item->prev = list->last;
  list->last->next = al_item;
  list->last = al_item;

  al_item = list->first;
}

bool allocation_list_remove(AllocationList *list, const void *item)
{
  if (!list)
  {
    return false;
  }

  AllocationListItem *al_item = list->first;

  while (al_item != NULL)
  {
    if (al_item->item != item)
    {
      al_item = al_item->next;
      continue;
    }

    list->size--;

    if (al_item->prev)
    {
      al_item->prev->next = al_item->next;
    }

    if (al_item->next)
    {
      al_item->next->prev = al_item->prev;
    }

    if (al_item == list->first)
    {
      list->first = al_item->next;
    }

    if (al_item == list->last)
    {
      list->last = al_item->prev;
    }

    delete_allocation_list_item(al_item);

    return true;
  }

  return false;
}

AllocationList *new_allocation_list()
{
  AllocationList *list = calloc(1, sizeof(AllocationList));

  list->append = allocation_list_append;
  list->remove = allocation_list_remove;

  return list;
}

void delete_allocation_list(AllocationList *list)
{
  if (!list)
  {
    return;
  }

  AllocationListItem *al_item = list->first;
  AllocationListItem *next = NULL;

  while (al_item != NULL)
  {
    next = al_item->next;
    delete_allocation_list_item(al_item);
    al_item = next;
  }

  free(list);
}
