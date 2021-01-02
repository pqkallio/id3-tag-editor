#include "hashset.h"
#include "linkedlist.h"
#include "../mem/mem.h"
#include "../util/hash.h"

unsigned long hashset_default_hash(const MemMap *memmap, const void *item)
{
  // Allocate enough bytes for a string representation of the pointer's address.
  // Since each byte is represented by two characters, the address takes sizeof(void *) * 2 characters.
  // Since it is preceded by "0x", we need two more bytes as well as extra null char in the end, thus +3.
  char *str = memmap->allocate(memmap, sizeof(void *) * 2 + 3, sizeof(char));

  sprintf(str, "%p", item);

  unsigned long str_hash = hash(str);

  memmap->free(memmap, str);

  return str_hash;
}

long hashset_default_compare_items(const void *hashset_item, const void *input_item)
{
  long diff = (long)hashset_item - (long)input_item;

  return diff;
}

const void *hashset_get_item_from_list(const HashSet *set, const LinkedList *list, const void *item)
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

  ll->append(ll, item);

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
    return HSE_NOT_FOUND;
  }

  const LinkedListItem *lli = ll->find(ll, item, NULL);
  const void *removed = ll->remove(ll, (LinkedListItem *)lli);

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

void delete_hashset(HashSet *set)
{
  if (!set)
  {
    return;
  }

  for (unsigned int i = 0; i < set->n_slots; i++)
  {
    LinkedList *ll = set->set[i];

    delete_linked_list(ll);
  }

  const MemMap *mem = set->memmap;

  mem->free(mem, set->set);
  mem->free(mem, set);
}
