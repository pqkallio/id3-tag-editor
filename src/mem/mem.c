#include <malloc.h>
#include "mem.h"

void free_entry(const void *entry)
{
    free((void *)entry);
}

void *memmap_allocate(const MemMap *map, const char *key, size_t num_items, size_t item_size)
{
    if (!map || !key)
        return NULL;

    void *allocation = calloc(num_items, item_size);

    hashmap_set(map->entries, key, allocation);

    return allocation;
}

void memmap_free(const MemMap *map, const char *key)
{
    if (!map || !key)
        return;

    const void *allocation = hashmap_get(map->entries, key);

    if (allocation)
    {
        hashmap_remove(map->entries, key);
        free_entry(allocation);
    }
}

void temp_alloc_area_create(const MemMap *map)
{
    if (!map)
    {
        return;
    }

    push_to_stack(map->temp_alloc, map->block_delimiter);
}

void *temp_alloc_add(const MemMap *map, size_t num_items, size_t item_size)
{
    if (!map)
    {
        return NULL;
    }

    void *allocation = calloc(num_items, item_size);

    push_to_stack(map->temp_alloc, allocation);

    return allocation;
}

void temp_alloc_area_destroy(const MemMap *map)
{
    if (!map)
    {
        return;
    }

    void *ptr = (void *)pop_stack(map->temp_alloc);

    while (ptr != map->block_delimiter && ptr != map->temp_alloc->bottom)
    {
        free(ptr);
        ptr = (void *)pop_stack(map->temp_alloc);
    }
}

MemMap *new_memmap()
{
    MemMap *map = calloc(1, sizeof(MemMap));

    Stack *stack = new_stack(1024);
    map->entries = new_hashmap();
    map->allocate = memmap_allocate;
    map->free = memmap_free;

    map->temp_alloc = stack;
    map->block_allocation_create = temp_alloc_area_create;
    map->block_allocation_push = temp_alloc_add;
    map->block_allocation_delete = temp_alloc_area_destroy;

    map->block_delimiter = malloc(sizeof(void *));

    return map;
}

void delete_memmap(MemMap *map)
{
    if (!map)
        return;

    if (map->entries)
    {
        hashmap_foreach(map->entries, free_entry);
        delete_hashmap(map->entries);
    }

    if (map->temp_alloc)
    {
        while (peek_stack(map->temp_alloc) != map->temp_alloc->bottom)
        {
            temp_alloc_area_destroy(map);
        }

        delete_stack(map->temp_alloc);
    }

    if (map->block_delimiter)
    {
        free(map->block_delimiter);
    }

    free(map);
}
