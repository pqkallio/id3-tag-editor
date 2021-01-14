#ifndef _ID3_HEADER_EDITOR_UTIL_SYSTEM_H
#define _ID3_HEADER_EDITOR_UTIL_SYSTEM_H

#include <stdbool.h>
#include "../dstructs/linkedlist.h"
#include "../mem/mem.h"

typedef enum file_type
{
  FOLDER_ = 4,
  FILE_ = 8
} FileType;

typedef struct _file_entry
{
  char *file_path;
  FileType type;
} FileEntry;

bool system_is_big_endian();
bool system_is_little_endian();
LinkedList *get_files_from_dir(const MemMap *memmap, const char *dir_name);

#endif /* _ID3_HEADER_EDITOR_UTIL_SYSTEM_H */
