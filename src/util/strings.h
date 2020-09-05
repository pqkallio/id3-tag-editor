#ifndef _ID3_TAG_EDITOR_UTIL_STRINGS_H
#define _ID3_TAG_EDITOR_UTIL_STRINGS_H

#include <malloc.h>
#include <string.h>

inline char* string_copy(const char* str)
{
  char* s = (char*)calloc(strlen(str) + 1, sizeof(char));

  strcpy(s, str);

  return s;
}

#endif /* _ID3_TAG_EDITOR_UTIL_STRINGS_H */
