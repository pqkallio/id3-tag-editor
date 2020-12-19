#ifndef _DSTRUCTS_HASHSET_ERRORS_H
#define _DSTRUCTS_HASHSET_ERRORS_H

#include <stdint.h>

typedef uint8_t HSError;

static const HSError HSE_SUCCESS = 0;
static const HSError HSE_NOT_FOUND = 1;
static const HSError HSE_ALREADY_EXISTS = 2;
static const HSError HSE_SET_OR_ITEM_NOT_PROVIDED = 3;

#endif /* _DSTRUCTS_HASHSET_ERRORS_H */
