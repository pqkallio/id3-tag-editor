#ifndef _MEM_H
#define _MEM_H

#include <stddef.h>
#include "../dstructs/hashmap.h"

typedef struct _mem_map {
    HashMap* entries;
} MemMap;

MemMap* new_memmap();
void delete_memmap(MemMap* map);

void* memmap_allocate(MemMap* map, const char* key, size_t num_items, size_t item_size);
void memmap_free(MemMap* map, const char* key);

#endif /* _MEM_H */
