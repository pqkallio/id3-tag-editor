#include <malloc.h>
#include "mem.h"

void free_entry(void* entry) {
    free(entry);
}

MemMap* new_memmap()
{
    MemMap* map = calloc(1, sizeof(MemMap));

    map->entries = new_hashmap();

    return map;
}

void delete_memmap(MemMap* map)
{
    if (!map) return;

    hashmap_foreach(map->entries, free_entry);
}

void* memmap_allocate(MemMap* map, const char* key, size_t num_items, size_t item_size)
{
    if (!map || !key) return;

    void* allocation = calloc(num_items, item_size);

    hashmap_set(map->entries, key, allocation);

    return allocation;
}

void memmap_free(MemMap* map, const char* key)
{
    if (!map || !key) return;

    void* allocation = hashmap_get(map->entries, key);

    if (allocation) free_entry(allocation);
}
