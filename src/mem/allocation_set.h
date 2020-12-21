#ifndef _MEM_ALLOCATION_SET_H
#define _MEM_ALLOCATION_SET_H

#include <stddef.h>
#include <stdbool.h>
#include "allocation_list.h"

typedef struct _allocation_set
{
  unsigned long size;
  unsigned long n_slots;

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
  unsigned long (*hash)(const void *item);

  bool (*add)(struct _allocation_set *set, const void *item);
  bool (*remove)(struct _allocation_set *set, const void *item);
  void (*clear)(struct _allocation_set *set);
  void (*foreach)(struct _allocation_set *set, void (*callback)(const void *item));

  AllocationList **set;
} AllocationSet;

AllocationSet *new_allocation_set();
void delete_allocation_set(AllocationSet *set);

#endif /* _MEM_ALLOCATION_SET_H */
