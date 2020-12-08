#include <malloc.h>
#include "mem.h"

void free_entry(void *entry)
{
    free(entry);
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

    hashmap_foreach(map->entries, free_entry);
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

    void *ptr = pop_stack(map->temp_alloc);

    while (ptr != NULL && ptr != &STACK_BOTTOM_)
    {
        free(ptr);
        ptr = pop_stack(map->temp_alloc);
    }
}
