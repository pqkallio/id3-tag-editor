#ifndef _DSTRUCTS_LINKEDLIST_H
#define _DSTRUCTS_LINKEDLIST_H

#include "./domain/tagv2.h"

typedef struct _linked_list_item {
  TagV2Frame* frame;
  struct _linked_list_item* prev;
  struct _linked_list_item* next;
} LinkedListItem;

typedef struct _linked_list {
  unsigned long size;
  struct _linked_list_item* first;
  struct _linked_list_item* last;
  void (*append)(struct _linked_list* list, TagV2Frame* frame);
  void (*remove)(struct _linked_list* list, char* frame_id);
} LinkedList;

LinkedList* new_linked_list();
void delete_linked_list(LinkedList* list);

#endif /* _DSTRUCTS_LINKEDLIST_H */
