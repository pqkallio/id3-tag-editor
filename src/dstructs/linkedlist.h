#ifndef _DSTRUCTS_LINKEDLIST_H
#define _DSTRUCTS_LINKEDLIST_H

typedef struct _linked_list_item {
  char* key;
  const void* item;
  struct _linked_list_item* prev;
  struct _linked_list_item* next;
} LinkedListItem;

typedef struct _linked_list {
  unsigned long size;
  struct _linked_list_item* first;
  struct _linked_list_item* last;
  void (*append)(struct _linked_list* list, const char* key, const void* item);
  unsigned int (*remove)(struct _linked_list* list, const char* key);
} LinkedList;

LinkedList* new_linked_list();
void delete_linked_list(LinkedList* list);

#endif /* _DSTRUCTS_LINKEDLIST_H */
