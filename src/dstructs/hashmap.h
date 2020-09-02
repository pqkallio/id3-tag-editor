#ifndef _DSTRUCTS_HASHMAP_H
#define _DSTRUCTS_HASHMAP_H

#include "linkedlist.h"
#include "../domain/tagv2.h"

typedef struct _hashmap {
  unsigned long size;
  unsigned long n_slots;

  LinkedList** map;
} HashMap;

HashMap* new_hashmap();
void delete_hashmap(HashMap* map);

TagV2Frame* get(const HashMap* map, const char* key);
void set(HashMap* map, const char* key, const TagV2Frame* value);
void remove(HashMap* map, const char* key);

#endif /* _DSTRUCTS_HASHMAP_H */
