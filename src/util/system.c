#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include "system.h"

bool system_is_big_endian()
{
    union
    {
        uint32_t i;
        char c[4];
    } bint = {0x01020304};

    return bint.c[0] == 1;
}

bool system_is_little_endian()
{
    return !system_is_big_endian();
}

LinkedList *get_files_from_dir(const MemMap *memmap, const char *dir_name)
{
    LinkedList *files = new_linked_list(memmap);
    struct dirent *dir;

    DIR *d = opendir(dir_name);

    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            if ((dir->d_type != FOLDER_ && dir->d_type != FILE_) ||
                !strcmp(".", dir->d_name) ||
                !strcmp("..", dir->d_name))
            {
                continue;
            }

            char *fname = dir->d_name;
            char *filename = memmap->allocate(memmap, 1, sizeof(char) * strlen(fname) + 1);
            strcpy(filename, fname);

            FileEntry *entry = memmap->allocate(memmap, 1, sizeof(FileEntry));
            entry->file_path = filename;
            entry->type = dir->d_type;
            files->append(files, entry);
        }

        closedir(d);
    }

    return files;
}
