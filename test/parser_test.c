#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <CUnit/CUnit.h>
#include "parser_test.h"
#include "src/parser/parser.h"
#include "test_utils.h"
#include "src/dstructs/hashmap.h"
#include "src/mem/mem.h"
#include "id3_v2_tag_mock.h"

FILE *fp = NULL;

void test_id3v2_parser()
{
  fp = open_test_file_in_read_write_mode();

  if (!fp)
  {
    return;
  }

  write_data_to_test_file_and_rewind(id3_v2_tag_mock, 6240);

  const char *expected_ids[10] = {
      "510FB07ECADF79C34108BD62A245AB11",
      "7F306CEA84844A30B1B390B8A165CBF2",
      "747CF0BEDB6A831E73C0009506ABA785",
      "DC93EFE0193A904A6EA0038F064F82FA",
      "4BF2B928DCBC4650FCC56603718D09CD",
      "21F82EB9B3BE0422DE87B1E556B360A1",
      "A699C983D68F54D3553AD747C7E09468",
      "59C3230D02A6AEE1AD6047049327A6B9",
      "C0EFBD9700916CD4CBC39B932431FB7D",
      "F49E0B76C5B9D7739A1FF7AD461E6808"};

  TagV2 *actual = parseMP3(&DEFAULT_MEMMAP, fp);

  CU_ASSERT_EQUAL(actual->frames->size, 10);

  const TagV2Frame *frame = get_tag_v2_frame(actual, "510FB07ECADF79C34108BD62A245AB11");
  CU_ASSERT_PTR_NOT_NULL_FATAL(frame);

  const TagV2FrameHeader *header = &frame->header;
  CU_ASSERT_STRING_EQUAL(frame->body, "CD 1");
  CU_ASSERT_EQUAL(header->size, 5);
  CU_ASSERT_EQUAL(header->has_zero_byte, 1);

  frame = get_tag_v2_frame(actual, "TIT2");
  CU_ASSERT_PTR_NOT_NULL_FATAL(frame);

  header = &frame->header;
  CU_ASSERT_STRING_EQUAL(frame->body, "Tn3mnn!)Hbwhb!tufmmbub(");
  CU_ASSERT_EQUAL(header->size, 24);
  CU_ASSERT_EQUAL(header->has_zero_byte, 1);

  frame = get_tag_v2_frame(actual, "TPE1");
  CU_ASSERT_PTR_NOT_NULL_FATAL(frame);

  header = &frame->header;
  CU_ASSERT_STRING_EQUAL(frame->body, "Hbwhb!tufmmbub");
  CU_ASSERT_EQUAL(header->size, 15);
  CU_ASSERT_EQUAL(header->has_zero_byte, 1);

  frame = get_tag_v2_frame(actual, "TLEN");
  CU_ASSERT_PTR_NOT_NULL_FATAL(frame);

  header = &frame->header;
  CU_ASSERT_STRING_EQUAL(frame->body, "64506");
  CU_ASSERT_EQUAL(header->size, 7);
  CU_ASSERT_EQUAL(header->has_zero_byte, 1);

  frame = get_tag_v2_frame(actual, "TCON");
  CU_ASSERT_PTR_NOT_NULL_FATAL(frame);

  header = &frame->header;
  CU_ASSERT_STRING_EQUAL(frame->body, "Mhoovombvmv");
  CU_ASSERT_EQUAL(header->size, 12);
  CU_ASSERT_EQUAL(header->has_zero_byte, 1);

  frame = get_tag_v2_frame(actual, "TCOM");
  CU_ASSERT_PTR_NOT_NULL_FATAL(frame);

  header = &frame->header;
  CU_ASSERT_STRING_EQUAL(frame->body, "Boesfbt!Tdivm{f");
  CU_ASSERT_EQUAL(header->size, 16);
  CU_ASSERT_EQUAL(header->has_zero_byte, 1);

  frame = get_tag_v2_frame(actual, "TRCK");
  CU_ASSERT_PTR_NOT_NULL_FATAL(frame);

  header = &frame->header;
  CU_ASSERT_STRING_EQUAL(frame->body, "1");
  CU_ASSERT_EQUAL(header->size, 2);
  CU_ASSERT_EQUAL(header->has_zero_byte, 1);

  close_and_remove_test_file();

  delete_tag_v2(actual);
}
