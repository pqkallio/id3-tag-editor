#ifndef _MEM_H
#define _MEM_H

#include <stddef.h>
#include "../dstructs/hashmap.h"
#include "../dstructs/stack.h"

typedef struct _mem_map
{
    HashMap *entries;
    Stack *temp_alloc;
    void *block_delimiter;
    void *(*allocate)(const struct _mem_map *map, const char *key, size_t num_items, size_t item_size);
    void (*free)(const struct _mem_map *map, const char *key);
    void (*block_allocation_create)(const struct _mem_map *map);
    void *(*block_allocation_push)(const struct _mem_map *map, size_t num_items, size_t item_size);
    void (*block_allocation_delete)(const struct _mem_map *map);
} MemMap;

MemMap *new_memmap();
void delete_memmap(MemMap *map);

#endif /* _MEM_H */
