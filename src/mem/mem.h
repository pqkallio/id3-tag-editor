#ifndef _MEM_H
#define _MEM_H

#include <stddef.h>
#include "../dstructs/hashmap.h"
#include "../dstructs/stack.h"

typedef struct _mem_map
{
    HashMap *entries;
    Stack *temp_alloc;
} MemMap;

MemMap *new_memmap();
void delete_memmap(MemMap *map);

void *memmap_allocate(MemMap *map, const char *key, size_t num_items, size_t item_size);
void memmap_free(MemMap *map, const char *key);

void temp_alloc_area_create(MemMap *map);
void *temp_alloc_add(MemMap *map, size_t num_items, size_t item_size);
void temp_alloc_area_destroy(MemMap *map);

#endif /* _MEM_H */
