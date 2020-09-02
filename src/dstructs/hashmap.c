#include <malloc.h>
#include <string.h>
#include "hashmap.h";

/* djb2 * This algorithm was first reported by Dan Bernstein many years ago in comp.lang.c */
unsigned long hash(const char *str)
{
    unsigned long hash = 5381;

    int c;

    while (c = *str++) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

TagV2Frame* get(const HashMap* map, const char* item)
{
    if (!map || !item) {
        return NULL;
    }

    unsigned long item_hash = hash(item) % map->n_slots;

    LinkedList* list = map->map[item_hash];

    if (!list) {
        return NULL;
    }

    LinkedListItem* ll_item = list->first;

    while (ll_item) {
        if (ll_item->frame && !strcmp(item, ll_item->frame->header.id)) {
            return ll_item->frame;
        }

        ll_item = ll_item->next;
    }

    return NULL;
}

void set(HashMap* map, const char* key, const TagV2Frame* value)
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

    TagV2Frame* copy = calloc(1, sizeof(TagV2Frame));
    memcpy(copy, value, sizeof(TagV2Frame));

    ll->append(ll, copy);

    map->size++;
}

void remove(HashMap* map, const char* key)
{
    if (!map || !key) {
        return;
    }

    unsigned long hash_key = hash(key);

    LinkedList* ll = map->map[hash_key];

    if (!ll) {
        return;
    }

    if (ll->remove(ll, key)) map->size--;
}

HashMap* new_hashmap()
{
    HashMap* map = calloc(1, sizeof(HashMap));

    map->n_slots = 100;
    map->map = calloc(map->n_slots, sizeof(LinkedList*));

    return map;
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
