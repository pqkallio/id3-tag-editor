#include <malloc.h>
#include "mem.h"

static inline char *string_copy(const char *str)
{
    char *s = (char *)calloc(strlen(str) + 1, sizeof(char));

    strcpy(s, str);

    return s;
}

unsigned long allocation_set_hash(const void *item)
{
    char *str = calloc(sizeof(void *) + 1, sizeof(char));

    unsigned long str_hash = hash(str);

    free(str);

    return str_hash;
}

long compare_items(const void *set_item, const void *input_item)
{
    long diff = set_item - input_item;

    return diff;
}

AllocationListItem *new_linked_list_item(const char *key, const void *item)
{
    AllocationListItem *ll_item = calloc(1, sizeof(AllocationListItem));
    char *item_key = string_copy(key);

    ll_item->item = item;
    ll_item->key = item_key;

    return ll_item;
}

void delete_linked_list_item(AllocationListItem *item)
{
    if (!item)
        return;

    free(item->key);
    free(item);
}

void linked_list_append(AllocationList *list, const char *key, const void *item)
{
    if (!list)
    {
        return;
    }

    AllocationListItem *ll_item = new_linked_list_item(key, item);

    list->size++;

    if (list->first == NULL)
    {
        list->first = ll_item;
    }

    if (list->last == NULL)
    {
        list->last = ll_item;

        return;
    }

    ll_item->prev = list->last;
    list->last->next = ll_item;
    list->last = ll_item;

    ll_item = list->first;
}

bool linked_list_remove(AllocationList *list, const char *key)
{
    if (!list)
    {
        return false;
    }

    AllocationListItem *ll_item = list->first;

    while (ll_item != NULL)
    {
        if (strcmp(ll_item->key, key))
        {
            ll_item = ll_item->next;
            continue;
        }

        list->size--;

        if (ll_item->prev)
        {
            ll_item->prev->next = ll_item->next;
        }

        if (ll_item->next)
        {
            ll_item->next->prev = ll_item->prev;
        }

        if (ll_item == list->first)
        {
            list->first = ll_item->next;
        }

        if (ll_item == list->last)
        {
            list->last = ll_item->prev;
        }

        delete_linked_list_item(ll_item);

        return true;
    }

    return false;
}

AllocationList *new_linked_list()
{
    AllocationList *list = calloc(1, sizeof(AllocationList));

    list->append = linked_list_append;
    list->remove = linked_list_remove;

    return list;
}

void delete_linked_list(AllocationList *list)
{
    if (!list)
    {
        return;
    }

    AllocationListItem *ll_item = list->first;
    AllocationListItem *next = NULL;

    while (ll_item != NULL)
    {
        next = ll_item->next;
        delete_linked_list_item(ll_item);
        ll_item = next;
    }

    free(list);
}
const void *allocation_set_get_item_from_list(const AllocationSet *set, const AllocationList *list, void *item)
{
    AllocationListItem *ll_item = list->first;

    while (ll_item && ll_item->item)
    {
        long items_differ = set->compare_items(ll_item->item, item);

        if (!items_differ)
        {
            return ll_item->item;
        }

        ll_item = ll_item->next;
    }

    return NULL;
}

const void *allocation_set_get(const AllocationSet *set, const void *item)
{
    if (!set || !item)
    {
        return NULL;
    }

    unsigned long item_hash = set->hash(item) % set->n_slots;

    AllocationList *list = set->set[item_hash];

    if (!list)
    {
        return NULL;
    }

    return allocation_set_get_item_from_list(set, list, item);
}

bool allocation_set_add(AllocationSet *set, const void *item)
{
    if (!set || !item)
    {
        return false;
    }

    unsigned long hash_key = set->hash(item) % set->n_slots;

    AllocationList *ll = set->set[hash_key];

    if (!ll)
    {
        set->set[hash_key] = new_linked_list();
        ll = set->set[hash_key];
    }

    if (allocation_set_get_item_from_list(set, ll, item))
    {
        return false;
    }

    ll->append(ll, item, item);

    set->size++;

    return true;
}

bool hashset_remove(AllocationSet *set, const void *item)
{
    if (!set || !item)
    {
        return false;
    }

    unsigned long hash_key = set->hash(item) % set->n_slots;

    AllocationList *ll = set->set[hash_key];

    if (!ll)
    {
        return;
    }

    unsigned int removed = ll->remove(ll, item);

    if (!removed)
    {
        return false;
    }

    set->size--;

    return true;
}

AllocationSet *new_hashset()
{
    AllocationSet *as = calloc(1, sizeof(AllocationSet));

    long (*compare_items)(const void *set_item, const void *input_item) =
        compare_items ? compare_items : compare_items;

    as->n_slots = 100;
    as->compare_items = compare_items;
    as->hash = allocation_set_hash;
    as->set = calloc(100, sizeof(AllocationList *));
    as->size = 0;

    as->add = allocation_set_add;
    as->get = allocation_set_get;
    as->remove = hashset_remove;

    return as;
}

void free_entry(const void *entry)
{
    free((void *)entry);
}

void *memmap_allocate(const MemMap *map, size_t num_items, size_t item_size)
{
    if (!map)
        return NULL;

    void *allocation = calloc(num_items, item_size);

    map->entries->add(map->entries, allocation);

    return allocation;
}

MemError memmap_free(const MemMap *map, const void *item)
{
    if (!map || !item)
        return MEME_MEMMAP_OR_ITEM_NOT_PROVIDED;

    bool removed = map->entries->remove(map->entries, item);

    if (!removed)
    {
        return MEME_INVALID_FREE;
    }

    free_entry(item);

    return MEME_SUCCESS;
}

void clear_memmap(const MemMap *map)
{
    if (!map)
        return;

    AllocationSet *e = map->entries;

    if (e)
    {
        hashmap_foreach(e, free_entry);
        delete_hashmap(e);
    }
}

MemMap *new_memmap()
{
    MemMap *map = calloc(1, sizeof(MemMap));

    map->entries = new_allocation_set();
    map->allocate = memmap_allocate;
    map->free = memmap_free;
    map->clear = clear_memmap;

    map->block_delimiter = malloc(sizeof(void *));

    return map;
}

void delete_memmap(MemMap *map)
{
    map->clear(map);

    if (map->block_delimiter)
    {
        free(map->block_delimiter);
    }

    free(map);
}
