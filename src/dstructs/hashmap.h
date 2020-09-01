#ifndef _DSTRUCTS_HASHMAP_H
#define _DSTRUCTS_HASHMAP_H

#include "linkedlist.h"

typedef struct _hashmap {
  unsigned long size;
  unsigned long n_slots;

  LinkedList* map;
} HashMap;

void* get(HashMap* map, void* item);

#endif /* _DSTRUCTS_HASHMAP_H */
