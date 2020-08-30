#ifndef _ID3_HEADER_EDITOR_UTIL_BINARY_H
#define _ID3_HEADER_EDITOR_UTIL_BINARY_H

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

#define XOR_SWAP_UNSAFE(a, b) ((a) ^= (b), (b) ^= (a), (a) ^= (b))
#define XOR_SWAP_SAFE(a, b) ((&(a) == &(b)) ? (a) : XOR_SWAP_UNSAFE(a, b))

unsigned int remove_padding(char* data, size_t data_len, size_t padding_amount_per_char);
void read_int(uint32_t* target, FILE* origin, int int_endianess, int cpu_endianess);
void read_little_endian_int(uint32_t *target, FILE* origin);
void read_big_endian_int(uint32_t *target, FILE* origin);

#endif /* _ID3_HEADER_EDITOR_UTIL_BINARY_H */
