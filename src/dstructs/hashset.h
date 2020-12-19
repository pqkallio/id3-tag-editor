#ifndef _DSTRUCTS_HASHSET_H
#define _DSTRUCTS_HASHSET_H

#include "linkedlist.h"
#include "src/mem/mem.h"
#include "hashset_errors.h"

static const unsigned long DEFAULT_N_SLOTS = 100;

typedef struct _hashset
{
  unsigned long size;
  unsigned long n_slots;
  const MemMap *memmap;

  /**
   * Used to compare items in the set with input objects, e.g. when adding items to or getting items from the set.
   *
   * Should return
   * - a negative value if the hash_set item is less than the input_item,
   * - 0 if the items are equal, or
   * - a positive value if the hash_set item is greater than the input_item.
   * The distance from 0 represents the distance between the compared objects.
   */
  long (*compare_items)(const void *hashset_item, const void *input_item);

  /**
   * Used for hashing items when retrieving items from or adding items to the set.
   */
  unsigned long (*hash)(const MemMap *memmap, const void *item);

  const void *(*get)(struct _hashset *set, const void *item);
  HSError (*add)(const struct _hashset *set, const void *item);
  HSError (*remove)(const struct _hashset *set, const void *item);

  LinkedList **set;
} HashSet;

/**
 * Allocates a new HashSet with default size and returns a pointer to it.
 *
 * - If *memmap is NULL, builtin allocation functions calloc and free are used for allocating data from the heap.
 * - If *hash is NULL, djb2 hashing function is used by converting the void pointer's address into a char string and
 *   Using it to generate the hash.
 * - If compare items is NULL, the default of comparing two void pointer's addresses is used.
 */
HashSet *new_hashset(
    const MemMap *memmap,
    unsigned long (*hash)(const void *item),
    long (*compare_items)(const void *hashset_item, const void *input_item));

/**
 * Same as with *new_hashset, but the number of slots it uses for allocating the data can be adjusted.
 * More slots equals greater efficiency and greater memory consumption (it's a tradeoff).
 */
HashSet *new_hashset_with_size(
    const MemMap *memmap,
    unsigned long (*hash)(const void *item),
    long (*compare_items)(const void *hashset_item, const void *input_item),
    unsigned long n_slots);

/**
 * Deletes the HashSet, releasing all its resources.
 */
void delete_hashset(HashSet *set);

#endif /* _DSTRUCTS_HASHSET_H */
