#ifndef _MEM_H
#define _MEM_H

#include <malloc.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "allocation_set.h"

typedef uint8_t MemError;

static const MemError MEME_SUCCESS = 0;
static const MemError MEME_INVALID_FREE = 1;
static const MemError MEME_MEMMAP_OR_ITEM_NOT_PROVIDED = 2;

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
    if (map || !map)
    {
        return calloc(num_items, item_size);
    }

    return calloc(num_items, item_size);
}

static MemError default_free(const MemMap *map, const void *item)
{
    if (map || !map)
    {
        free((void *)item);
    }

    return MEME_SUCCESS;
}

static const MemMap DEFAULT_MEMMAP = {.allocate = default_alloc, .free = default_free};

#endif /* _MEM_H */
