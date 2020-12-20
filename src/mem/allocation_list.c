#include <malloc.h>
#include <string.h>
#include "allocation_list.h"

static inline char *mem_string_copy(const char *str)
{
  char *s = (char *)calloc(strlen(str) + 1, sizeof(char));

  strcpy(s, str);

  return s;
}

AllocationListItem *new_allocation_list_item(const char *key, const void *item)
{
  AllocationListItem *ll_item = calloc(1, sizeof(AllocationListItem));
  char *item_key = mem_string_copy(key);

  ll_item->item = item;
  ll_item->key = item_key;

  return ll_item;
}

void delete_allocation_list_item(AllocationListItem *item)
{
  if (!item)
    return;

  free(item->key);
  free(item);
}

void allocation_list_append(AllocationList *list, const char *key, const void *item)
{
  if (!list)
  {
    return;
  }

  AllocationListItem *ll_item = new_allocation_list_item(key, item);

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

bool allocation_list_remove(AllocationList *list, const char *key)
{
  if (!list)
  {
    return false;
  }

  AllocationListItem *ll_item = list->first;

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

    delete_allocation_list_item(ll_item);

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

  AllocationListItem *ll_item = list->first;
  AllocationListItem *next = NULL;

  while (ll_item != NULL)
  {
    next = ll_item->next;
    delete_allocation_list_item(ll_item);
    ll_item = next;
  }

  free(list);
}
