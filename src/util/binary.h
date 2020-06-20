#ifndef _ID3_HEADER_EDITOR_UTIL_BINARY_H
#define _ID3_HEADER_EDITOR_UTIL_BINARY_H

#include <stddef.h>

unsigned int remove_padding(char* data, size_t data_len, size_t padding_amount_per_char);

#endif /* _ID3_HEADER_EDITOR_UTIL_BINARY_H */
