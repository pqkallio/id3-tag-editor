#ifndef _DSTRUCTS_HASHMAP_H
#define _DSTRUCTS_HASHMAP_H

#include <stdbool.h>
#include "linkedlist.h"
#include "../mem/mem.h"

typedef struct _hashmap
{
  const MemMap *memmap;
  unsigned long size;
  unsigned long n_slots;

  bool (*set)(struct _hashmap *map, const char *key, const void *item);
  const void *(*get)(struct _hashmap *map, const char *key);
  const void *(*remove)(struct _hashmap *map, const char *key);
  void (*clear)(struct _hashmap *map);
  void (*foreach)(struct _hashmap *map, void (*callback)(const void *item));

  LinkedList **map;
} HashMap;

HashMap *new_hashmap(const MemMap *memmap);
HashMap *new_hashmap_with_size(const MemMap *memmap, unsigned long size);
void delete_hashmap(HashMap *map);

#endif /* _DSTRUCTS_HASHMAP_H */
