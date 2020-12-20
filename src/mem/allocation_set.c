#include <malloc.h>
#include "allocation_set.h"
#include "../util/hash.h"

unsigned long allocation_set_hash(const void *item)
{
  // Allocate enough bytes for a string representation of the pointer's address.
  // Since each byte is represented by two characters, the address takes sizeof(void *) * 2 characters.
  // Since it is preceded by "0x", we need two more bytes as well as extra null char in the end, thus +3.
  char *str = calloc(sizeof(void *) * 2 + 3, sizeof(char));

  sprintf(str, "%p", item);

  unsigned long str_hash = hash(str);

  free(str);

  return str_hash;
}

long mem_compare_items(const void *set_item, const void *input_item)
{
  long diff = (long)set_item - (long)input_item;

  return diff;
}

const void *allocation_set_get_item_from_list(const AllocationSet *set, const AllocationList *list, const void *item)
{
  AllocationListItem *ll_item = list->first;

  while (ll_item && ll_item->item)
  {
    long items_differ = set->compare_items(ll_item->item, item);

    if (!items_differ)
    {
      return ll_item->item;
    }

    ll_item = ll_item->next;
  }

  return NULL;
}

bool allocation_set_add(AllocationSet *set, const void *item)
{
  if (!set || !item)
  {
    return false;
  }

  unsigned long hash_key = set->hash(item) % set->n_slots;

  AllocationList *ll = set->set[hash_key];

  if (!ll)
  {
    set->set[hash_key] = new_allocation_list();
    ll = set->set[hash_key];
  }

  if (allocation_set_get_item_from_list(set, ll, item))
  {
    return false;
  }

  ll->append(ll, item, item);

  set->size++;

  return true;
}

bool allocation_set_remove(AllocationSet *set, const void *item)
{
  if (!set || !item)
  {
    return false;
  }

  unsigned long hash_key = set->hash(item) % set->n_slots;

  AllocationList *ll = set->set[hash_key];

  if (!ll)
  {
    return false;
  }

  unsigned int removed = ll->remove(ll, item);

  if (!removed)
  {
    return false;
  }

  set->size--;

  return true;
}

void delete_allocation_set(AllocationSet *set)
{
  if (!set)
  {
    return;
  }

  for (unsigned int i = 0; i < set->n_slots; i++)
  {
    AllocationList *ll = set->set[i];

    delete_allocation_list(ll);
  }

  free(set->set);
  free(set);
}

void allocation_set_foreach(AllocationSet *set, void (*callback)(const void *item))
{
  if (!set || !callback)
  {
    return;
  }

  for (unsigned int i = 0; i < set->n_slots; i++)
  {
    AllocationList *ll = set->set[i];

    if (ll == NULL)
      continue;

    AllocationListItem *ll_item = ll->first;

    while (ll_item)
    {
      callback(ll_item->item);

      ll_item = ll_item->next;
    }
  }
}

AllocationSet *new_allocation_set()
{
  AllocationSet *as = calloc(1, sizeof(AllocationSet));

  as->n_slots = 100;
  as->compare_items = mem_compare_items;
  as->hash = allocation_set_hash;
  as->set = calloc(100, sizeof(AllocationList *));
  as->size = 0;

  as->add = allocation_set_add;
  as->remove = allocation_set_remove;
  as->foreach = allocation_set_foreach;

  return as;
}
