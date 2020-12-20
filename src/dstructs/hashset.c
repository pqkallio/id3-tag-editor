#include "hashset.h"
#include "linkedlist.h"
#include "../mem/mem.h"
#include "../util/hash.h"

unsigned long hashset_default_hash(const MemMap *memmap, const void *item)
{
  char *str = memmap->allocate(memmap, sizeof(void *) + 1, sizeof(char));

  unsigned long str_hash = hash(str);

  memmap->free(memmap, str);

  return str_hash;
}

long hashset_default_compare_items(const void *hashset_item, const void *input_item)
{
  long diff = hashset_item - input_item;

  return diff;
}

const void *hashset_get_item_from_list(const HashSet *set, const LinkedList *list, void *item)
{
  LinkedListItem *ll_item = list->first;

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

const void *hashset_get(const HashSet *set, const void *item)
{
  if (!set || !item)
  {
    return NULL;
  }

  unsigned long item_hash = set->hash(set->memmap, item) % set->n_slots;

  LinkedList *list = set->set[item_hash];

  if (!list)
  {
    return NULL;
  }

  return hashset_get_item_from_list(set, list, item);
}

HSError hashset_add(HashSet *set, const void *item)
{
  if (!set || !item)
  {
    return HSE_SET_OR_ITEM_NOT_PROVIDED;
  }

  unsigned long hash_key = set->hash(set->memmap, item) % set->n_slots;

  LinkedList *ll = set->set[hash_key];

  if (!ll)
  {
    set->set[hash_key] = new_linked_list(set->memmap);
    ll = set->set[hash_key];
  }

  if (hashset_get_item_from_list(set, ll, item))
  {
    return HSE_ALREADY_EXISTS;
  }

  ll->append(ll, item, item);

  set->size++;

  return HSE_SUCCESS;
}

HSError hashset_remove(HashSet *set, const void *item)
{
  if (!set || !item)
  {
    return HSE_SET_OR_ITEM_NOT_PROVIDED;
  }

  unsigned long hash_key = set->hash(set->memmap, item) % set->n_slots;

  LinkedList *ll = set->set[hash_key];

  if (!ll)
  {
    return;
  }

  unsigned int removed = ll->remove(ll, item);

  if (!removed)
  {
    return HSE_NOT_FOUND;
  }

  set->size--;

  return HSE_SUCCESS;
}

HashSet *new_hashset_with_size(
    const MemMap *memmap,
    unsigned long (*hasher)(const MemMap *memmap, const void *item),
    long (*comparator)(const void *hashset_item, const void *input_item),
    unsigned long n_slots)
{
  const MemMap *mem = memmap ? memmap : &DEFAULT_MEMMAP;
  HashSet *hashset = mem->allocate(mem, 1, sizeof(HashSet));

  hashset->n_slots = n_slots;
  hashset->memmap = mem;
  hashset->compare_items = comparator ? comparator : hashset_default_compare_items;
  hashset->hash = hasher ? hasher : hashset_default_hash;
  hashset->set = mem->allocate(mem, n_slots, sizeof(LinkedList *));
  hashset->size = 0;

  hashset->add = hashset_add;
  hashset->get = hashset_get;
  hashset->remove = hashset_remove;

  return hashset;
}

HashSet *new_hashset(
    const MemMap *memmap,
    unsigned long (*hash)(const MemMap *memmap, const void *item),
    long (*compare_items)(const void *hashset_item, const void *input_item))
{
  return new_hashset_with_size(memmap, hash, compare_items, DEFAULT_N_SLOTS);
}
