#ifndef _ID3_TAG_PARSER_TEST_UTILS_H
#define _ID3_TAG_PARSER_TEST_UTILS_H

#include <stdint.h>
#include <stdio.h>

FILE* open_test_file_in_read_write_mode();
void write_data_to_test_file_and_rewind(const uint16_t*, const size_t n);
int close_and_remove_test_file();

#endif /* _ID3_TAG_PARSER_TEST_UTILS_H */
