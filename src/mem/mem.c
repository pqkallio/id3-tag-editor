#include <malloc.h>
#include "mem.h"

void free_entry(const void *entry)
{
    free((void *)entry);
}

MemMap *new_memmap()
{
    MemMap *map = calloc(1, sizeof(MemMap));
    Stack *stack = calloc(1, sizeof(Stack));

    map->entries = new_hashmap();
    map->temp_alloc = stack;

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

    free(map);
}

void *memmap_allocate(MemMap *map, const char *key, size_t num_items, size_t item_size)
{
    if (!map || !key)
        return NULL;

    void *allocation = calloc(num_items, item_size);

    hashmap_set(map->entries, key, allocation);

    return allocation;
}

void memmap_free(MemMap *map, const char *key)
{
    if (!map || !key)
        return;

    const void *allocation = hashmap_get(map->entries, key);

    if (allocation)
        free_entry(allocation);
}

void temp_alloc_area_create(MemMap *map)
{
    if (!map)
    {
        return;
    }

    push_to_stack(map->temp_alloc, NULL);
}

void *temp_alloc_add(MemMap *map, size_t num_items, size_t item_size)
{
    if (!map)
    {
        return NULL;
    }

    void **allocation = calloc(num_items, item_size);

    for (unsigned int i = 0; i < num_items; i++)
    {
        push_to_stack(map->temp_alloc, allocation[i]);
    }

    return allocation;
}

void temp_alloc_area_destroy(MemMap *map)
{
    if (!map)
    {
        return;
    }

    void *ptr = (void *)pop_stack(map->temp_alloc);

    while (ptr != NULL && ptr != map->temp_alloc->bottom)
    {
        free(ptr);
        ptr = (void *)pop_stack(map->temp_alloc);
    }
}
