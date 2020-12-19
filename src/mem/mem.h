#ifndef _MEM_H
#define _MEM_H

#include <malloc.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef uint8_t MemError;

static const MemError MEME_SUCCESS = 0;
static const MemError MEME_INVALID_FREE = 1;
static const MemError MEME_MEMMAP_OR_ITEM_NOT_PROVIDED = 2;

typedef struct _allocation_list_item
{
    char *key;
    const void *item;
    struct _allocation_list_item *prev;
    struct _allocation_list_item *next;
} AllocationListItem;

typedef struct _allocation_list
{
    unsigned long size;
    AllocationListItem *first;
    AllocationListItem *last;

    void (*append)(struct _allocation_list *list, const char *key, const void *item);
    bool (*remove)(struct _allocation_list *list, const char *key);
} AllocationList;

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

    const void *(*get)(struct _allocation_set *set, const void *item);
    bool (*add)(const struct _allocation_set *set, const void *item);
    bool (*remove)(const struct _allocation_set *set, const void *item);

    AllocationList **set;
} AllocationSet;

typedef struct _mem_map
{
    AllocationSet *entries;
    void *block_delimiter;
    void *(*allocate)(const struct _mem_map *map, size_t num_items, size_t item_size);
    MemError (*free)(const struct _mem_map *map, const void *item);
    void (*clear)(const struct _mem_map *map);
} MemMap;

MemMap *new_memmap();
void delete_memmap(MemMap *map);

static void *default_alloc(const MemMap *map, size_t num_items, size_t item_size)
{
    map->clear(map);
    return calloc(num_items, item_size);
}

static MemError default_free(const MemMap *map, const void *item)
{
    map->clear(map);
    free((void *)item);

    return MEME_SUCCESS;
}

static const MemMap DEFAULT_MEMMAP = {.allocate = default_alloc, .free = default_free};

#endif /* _MEM_H */
