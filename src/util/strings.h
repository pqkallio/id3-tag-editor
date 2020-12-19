#ifndef _ID3_TAG_EDITOR_UTIL_STRINGS_H
#define _ID3_TAG_EDITOR_UTIL_STRINGS_H

#include <string.h>
#include "src/mem/mem.h"

static inline char *string_copy(const MemMap *memmap, const char *str)
{
  const MemMap *mem = memmap ? memmap : &DEFAULT_MEMMAP;

  char *s = (char *)mem->allocate(mem, strlen(str) + 1, sizeof(char));

  strcpy(s, str);

  return s;
}

#endif /* _ID3_TAG_EDITOR_UTIL_STRINGS_H */
