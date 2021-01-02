#ifndef _ID3_TAG_EDITOR_UTIL_STRINGS_H
#define _ID3_TAG_EDITOR_UTIL_STRINGS_H

#include <stdint.h>
#include <string.h>
#include "../mem/mem.h"

static inline char *string_copy(const MemMap *memmap, const char *str)
{
  const MemMap *mem = memmap ? memmap : &DEFAULT_MEMMAP;

  char *s = (char *)mem->allocate(mem, strlen(str) + 1, sizeof(char));

  strcpy(s, str);

  return s;
}

static inline void char_to_hex(char *dst, uint8_t val)
{
  char hi = (char)((val >> 4) & 0x0f);
  char lo = (char)(val & 0x0f);

  dst[0] = hi < 10 ? (char)(hi + 0x30) : (char)(hi - 10 + 0x41);
  dst[1] = lo < 10 ? (char)(lo + 0x30) : (char)(lo - 10 + 0x41);
}

#endif /* _ID3_TAG_EDITOR_UTIL_STRINGS_H */
