#include <malloc.h>
#include <string.h>
#include "hashmap.h"
#include "../util/hash.h"

void hashmap_foreach(HashMap *map, void (*callback)(const void *item))
{
    if (!map || !callback)
    {
        return;
    }

    for (unsigned int i = 0; i < map->n_slots; i++)
    {
        LinkedList *ll = map->map[i];

        if (ll == NULL)
            continue;

        LinkedListItem *ll_item = ll->first;

        while (ll_item)
        {
            callback(ll_item->item);

            ll_item = ll_item->next;
        }
    }
}

const void *hashmap_get(HashMap *map, const char *key)
{
    if (!map || !key)
    {
        return NULL;
    }

    unsigned long item_hash = hash(key) % map->n_slots;

    LinkedList *list = map->map[item_hash];

    if (!list)
    {
        return NULL;
    }

    LinkedListItem *ll_item = list->first;

    while (ll_item)
    {
        if (!ll_item->item)
        {
            continue;
        }

        HashMapEntry *entry = (HashMapEntry *)ll_item->item;

        if (!strcmp(key, entry->key))
        {
            return entry->item;
        }

        ll_item = ll_item->next;
    }

    return NULL;
}

bool hashmap_set(HashMap *map, const char *key, const void *value)
{
    if (!map || !key || !value)
    {
        return false;
    }

    if (map->get(map, key) != NULL)
    {
        return false;
    }

    unsigned long hash_key = hash(key) % map->n_slots;

    LinkedList *ll = map->map[hash_key];

    if (!ll)
    {
        map->map[hash_key] = new_linked_list(map->memmap);
        ll = map->map[hash_key];
    }

    HashMapEntry *entry = map->memmap->allocate(map->memmap, 1, sizeof(HashMapEntry));
    entry->key = string_copy(map->memmap, key);
    entry->item = value;

    ll->append(ll, entry);

    map->size++;

    return true;
}

int hashmap_entry_comparator(const LinkedListItem *ll_item, const void *item)
{
    HashMapEntry *entry = (HashMapEntry *)(ll_item->item);
    const char *key = (const char *)item;

    return strcmp(entry->key, key);
}

const void *hashmap_remove(HashMap *map, const char *key)
{
    if (!map || !key)
    {
        return NULL;
    }

    unsigned long hash_key = hash(key) % map->n_slots;

    LinkedList *ll = map->map[hash_key];

    if (!ll)
    {
        return NULL;
    }

    LinkedListItem *ll_item = (LinkedListItem *)ll->find(ll, key, hashmap_entry_comparator);
    const void *item = ll->remove(ll, ll_item);

    if (item)
        map->size--;

    return item;
}

void hashmap_clear(HashMap *map)
{
    for (unsigned int i = 0; i < map->n_slots; i++)
    {
        LinkedList *ll = map->map[i];

        delete_linked_list(ll);

        map->map[i] = NULL;
    }

    map->size = 0;
}

HashMap *new_hashmap_with_size(const MemMap *memmap, unsigned long size)
{
    const MemMap *mem = memmap ? memmap : &DEFAULT_MEMMAP;
    HashMap *map = mem->allocate(mem, 1, sizeof(HashMap));

    map->memmap = mem;
    map->n_slots = size;
    map->map = mem->allocate(mem, map->n_slots, sizeof(LinkedList *));

    map->foreach = hashmap_foreach;
    map->get = hashmap_get;
    map->remove = hashmap_remove;
    map->set = hashmap_set;
    map->clear = hashmap_clear;

    return map;
}

HashMap *new_hashmap(const MemMap *memmap)
{
    return new_hashmap_with_size(memmap, 100);
}

void delete_hashmap(HashMap *map)
{
    if (!map)
    {
        return;
    }

    for (unsigned int i = 0; i < map->n_slots; i++)
    {
        LinkedList *ll = map->map[i];

        delete_linked_list(ll);
    }

    const MemMap *mem = map->memmap;

    mem->free(mem, map->map);
    mem->free(mem, map);
}
