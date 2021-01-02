#include "mem.h"

extern void *default_alloc(const MemMap *map, size_t num_items, size_t item_size);

extern MemError default_free(const MemMap *map, const void *item);

void *memmap_allocate(const MemMap *map, size_t num_items, size_t item_size)
{
    if (!map)
        return NULL;

    void *allocation = calloc(num_items, item_size);

    map->entries->add(map->entries, allocation);

    return allocation;
}

MemError memmap_free(const MemMap *map, const void *item)
{
    if (!map || !item)
        return MEME_MEMMAP_OR_ITEM_NOT_PROVIDED;

    bool removed = map->entries->remove(map->entries, item);

    if (!removed)
    {
        return MEME_INVALID_FREE;
    }

    return MEME_SUCCESS;
}

void clear_memmap(const MemMap *map)
{
    if (!map)
        return;

    AllocationSet *e = map->entries;

    if (e)
    {
        e->clear(e);
    }
}

MemMap *new_memmap()
{
    MemMap *map = calloc(1, sizeof(MemMap));

    map->entries = new_allocation_set();
    map->allocate = memmap_allocate;
    map->free = memmap_free;
    map->clear = clear_memmap;

    map->block_delimiter = malloc(sizeof(void *));

    return map;
}

void delete_memmap(MemMap *map)
{
    if (!map)
    {
        return;
    }

    map->clear(map);

    delete_allocation_set(map->entries);

    if (map->block_delimiter)
    {
        free(map->block_delimiter);
    }

    free(map);
}
