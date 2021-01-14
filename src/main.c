#include <malloc.h>
#include <time.h>
#include <stdlib.h>
#include "util/system.h"
#include "mem/mem.h"
#include "dstructs/linkedlist.h"

int main()
{
    srand(time(NULL));
    MemMap *memmap = new_memmap();

    LinkedList *list = get_files_from_dir(memmap, ".");

    LinkedListItem *item = list->first;

    while (item)
    {
        FileEntry *entry = (FileEntry *)item->item;
        printf("%s, %d\n", entry->file_path, entry->type);
        item = item->next;
    }

    memmap->clear(memmap);

    delete_memmap(memmap);

    return 0;
}
