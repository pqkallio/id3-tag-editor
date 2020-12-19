#include "hash.h"

/* djb2 * This algorithm was first reported by Dan Bernstein many years ago in comp.lang.c */
unsigned long hash(const char *str)
{
  unsigned long hash = 5381;

  int c;

  while ((c = *str++))
  {
    hash = ((hash << 5) + hash) + c;
  }

  return hash;
}
