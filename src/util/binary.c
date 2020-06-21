#include <math.h>
#include "binary.h"

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
