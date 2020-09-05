#ifndef _DSTRUCTS_HASHMAP_H
#define _DSTRUCTS_HASHMAP_H

#include "linkedlist.h"

typedef struct _hashmap {
  unsigned long size;
  unsigned long n_slots;

  LinkedList** map;
} HashMap;

HashMap* new_hashmap();
HashMap* new_hashmap_with_size(unsigned long size);
void delete_hashmap(HashMap* map);

const void* get(const HashMap* map, const char* key);
void set(HashMap* map, const char* key, const void* value);
void hashmap_remove(HashMap* map, const char* key);

#endif /* _DSTRUCTS_HASHMAP_H */
