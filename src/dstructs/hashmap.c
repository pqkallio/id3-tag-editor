#include <malloc.h>
#include <string.h>
#include "hashmap.h"

/* djb2 * This algorithm was first reported by Dan Bernstein many years ago in comp.lang.c */
unsigned long hash(const char *str)
{
    unsigned long hash = 5381;

    int c;

    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

const void* hashmap_get(const HashMap* map, const char* key)
{
    if (!map || !key) {
        return NULL;
    }

    unsigned long item_hash = hash(key) % map->n_slots;

    LinkedList* list = map->map[item_hash];

    if (!list) {
        return NULL;
    }

    LinkedListItem* ll_item = list->first;

    while (ll_item) {
        if (ll_item->item && !strcmp(key, ll_item->key)) {
            return ll_item->item;
        }

        ll_item = ll_item->next;
    }

    return NULL;
}

void hashmap_set(HashMap* map, const char* key, const void* value)
{
    if (!map || !key || !value) {
        return;
    }

    unsigned long hash_key = hash(key) % map->n_slots;

    LinkedList* ll = map->map[hash_key];

    if (!ll) {
        map->map[hash_key] = new_linked_list();
        ll = map->map[hash_key];
    }

    ll->append(ll, key, value);

    map->size++;
}

void hashmap_remove(HashMap* map, const char* key)
{
    if (!map || !key) {
        return;
    }

    unsigned long hash_key = hash(key) % map->n_slots;

    LinkedList* ll = map->map[hash_key];

    if (!ll) {
        return;
    }

    if (ll->remove(ll, key)) map->size--;
}

HashMap* new_hashmap_with_size(unsigned long size)
{
    HashMap* map = calloc(1, sizeof(HashMap));

    map->n_slots = size;
    map->map = calloc(map->n_slots, sizeof(LinkedList*));

    return map;
}

HashMap* new_hashmap()
{
    return new_hashmap_with_size(100);
}


void delete_hashmap(HashMap* map)
{
    if (!map) {
        return;
    }

    for (unsigned int i = 0; i < map->n_slots; i++) {
        LinkedList* ll = map->map[i];

        delete_linked_list(ll);
    }

    free(map->map);

    free(map);
}

void hashmap_foreach(HashMap* map, void (*callback)(void* item))
{
    for (unsigned int i = 0; i < map->n_slots; i++) {
        LinkedList* ll = map->map[i];

        if (ll == NULL) continue;

        LinkedListItem* ll_item = ll->first;

        while (ll_item) {
            callback(ll_item->item);

            ll_item = ll_item->next;
        }
    }
}
