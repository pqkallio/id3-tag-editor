#include <stdlib.h>
#include "unique_id.h"
#include "strings.h"

/**
 * Generates a unique id of hexadecimal representation of a desired length.
 * The length must be divisible by two or no unique id is generated.
 */
void generate_unique_id(char *id_container, uint8_t length)
{
  // Check that the length is divisible by two.
  if (length % 2 != 0)
  {
    return;
  }

  for (uint8_t i = 0; i < length; i += 2)
  {
    uint8_t c = (uint8_t)(rand() % 255);

    char_to_hex(&(id_container[i]), c);
  }
}
