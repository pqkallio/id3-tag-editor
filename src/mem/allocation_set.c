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
  AllocationListItem *al_item = list->first;

  while (al_item && al_item->item)
  {
    long items_differ = set->compare_items(al_item->item, item);

    if (!items_differ)
    {
      return al_item->item;
    }

    al_item = al_item->next;
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

  AllocationList *al = set->set[hash_key];

  if (!al)
  {
    set->set[hash_key] = new_allocation_list();
    al = set->set[hash_key];
  }

  if (allocation_set_get_item_from_list(set, al, item))
  {
    return false;
  }

  al->append(al, item);

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

  AllocationList *al = set->set[hash_key];

  if (!al)
  {
    return false;
  }

  unsigned int removed = al->remove(al, item);

  if (!removed)
  {
    return false;
  }

  if (al->size == 0)
  {
    delete_allocation_list(al);
    set->set[hash_key] = NULL;
  }

  set->size--;

  return true;
}

void clear_allocation_set(AllocationSet *set)
{
  for (unsigned int i = 0; i < set->n_slots; i++)
  {
    AllocationList *al = set->set[i];

    delete_allocation_list(al);

    set->set[i] = NULL;
  }

  set->size = 0;
}

void delete_allocation_set(AllocationSet *set)
{
  if (!set)
  {
    return;
  }

  clear_allocation_set(set);

  free(set->set);
  free(set);
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
  as->clear = clear_allocation_set;

  return as;
}
