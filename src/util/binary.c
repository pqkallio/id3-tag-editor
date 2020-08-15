#include <math.h>
#include "binary.h"
#include "system.h"
#include <stdio.h>

unsigned int remove_padding(char* data, size_t data_len, size_t significant_bits_per_char)
{
    if (significant_bits_per_char <= 0) return 0;

    unsigned char mask = pow(2, significant_bits_per_char - 1) - 1;
    unsigned int output = 0;

    for (size_t i = 0, j = data_len - 1; i < data_len; i++, j--) {
        output |= (mask & data[i]) << j * significant_bits_per_char;
    }

    return output;
}

void read_int(uint32_t* target, FILE* origin, int int_endianess, int cpu_endianess)
{
    size_t uint32size = sizeof(uint32_t);

    if (int_endianess == cpu_endianess) {
        fread(target, uint32size, 1, origin);

        return;
    }

    union {
        uint32_t i;
        char c[4];
    } data;

    fread(&data.i, uint32size, 1, origin);

    for (size_t i = 0, j = uint32size - 1; i < uint32size / 2; i++, j--) {
        XOR_SWAP_UNSAFE(data.c[i], data.c[j]);
    }

    *target = data.i;
}

void read_little_endian_int(uint32_t *target, FILE* origin)
{
    read_int(target, origin, 1, CPU_IS_LITTLE_ENDIAN);
}

void read_big_endian_int(uint32_t *target, FILE* origin)
{
    read_int(target, origin, 1, CPU_IS_BIG_ENDIAN);
}
