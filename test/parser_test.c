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

typedef struct _parser_test_data
{
  const char *id;
  const char *header_id;
  const char *frame_body;
  const uint32_t header_size;
  const unsigned char header_has_zero_byte;
} ParserTestData;

void test_id3v2_parser()
{
  fp = open_test_file_in_read_write_mode();

  if (!fp)
  {
    return;
  }

  write_data_to_test_file_and_rewind(id3_v2_tag_mock, 6240);

  srand(42);

  const char tit_id[] = "510FB07ECADF79C34108BD62A245AB11";
  const char tit_hid[] = "TIT2";
  const char tit_fb[] = "Tn3mnn!)Hbwhb!tufmmbub(";

  const char mcdi_id[] = "7F306CEA84844A30B1B390B8A165CBF2";
  const char mcdi_hid[] = "MCDI";
  const char mcdi_fb[] = "6";

  const char trck_id[] = "747CF0BEDB6A831E73C0009506ABA785";
  const char trck_hid[] = "TRCK";
  const char trck_fb[] = "1";

  const char priv1_id[] = "DC93EFE0193A904A6EA0038F064F82FA";
  const char priv1_hid[] = "PRIV";
  const char priv1_fb[] = "PeakValue";

  const char priv2_id[] = "4BF2B928DCBC4650FCC56603718D09CD";
  const char priv2_hid[] = "PRIV";
  const char priv2_fb[] = "AverageLevel";

  const char alb_id[] = "21F82EB9B3BE0422DE87B1E556B360A1";
  const char alb_hid[] = "TALB";
  const char alb_fb[] = "CD 1";

  const char con_id[] = "A699C983D68F54D3553AD747C7E09468";
  const char con_hid[] = "TCON";
  const char con_fb[] = "Mhoovombvmv";

  const char com_id[] = "59C3230D02A6AEE1AD6047049327A6B9";
  const char com_hid[] = "TCOM";
  const char com_fb[] = "Boesfbt!Tdivm{f";

  const char tpe_id[] = "C0EFBD9700916CD4CBC39B932431FB7D";
  const char tpe_hid[] = "TPE1";
  const char tpe_fb[] = "Hbwhb!tufmmbub";

  const char len_id[] = "F49E0B76C5B9D7739A1FF7AD461E6808";
  const char len_hid[] = "TLEN";
  const char len_fb[] = "64506";

  const ParserTestData test_data[] = {
      {.id = &tit_id, .header_id = &tit_hid, .frame_body = &tit_fb, .header_size = 24, .header_has_zero_byte = 1},
      {.id = &mcdi_id, .header_id = &mcdi_hid, .frame_body = &mcdi_fb, .header_size = 1176, .header_has_zero_byte = 0},
      {.id = &trck_id, .header_id = &trck_hid, .frame_body = &trck_fb, .header_size = 2, .header_has_zero_byte = 1},
      {.id = &priv1_id, .header_id = &priv1_hid, .frame_body = &priv1_fb, .header_size = 14, .header_has_zero_byte = 0},
      {.id = &priv2_id, .header_id = &priv2_hid, .frame_body = &priv2_fb, .header_size = 17, .header_has_zero_byte = 0},
      {.id = &alb_id, .header_id = &alb_hid, .frame_body = &alb_fb, .header_size = 5, .header_has_zero_byte = 1},
      {.id = &con_id, .header_id = &con_hid, .frame_body = &con_fb, .header_size = 12, .header_has_zero_byte = 1},
      {.id = &com_id, .header_id = &com_hid, .frame_body = &com_fb, .header_size = 16, .header_has_zero_byte = 1},
      {.id = &tpe_id, .header_id = &tpe_hid, .frame_body = &tpe_fb, .header_size = 15, .header_has_zero_byte = 1},
      {.id = &len_id, .header_id = &len_hid, .frame_body = &len_fb, .header_size = 7, .header_has_zero_byte = 1},
  };

  TagV2 *actual = parseMP3(&DEFAULT_MEMMAP, fp);
  CU_ASSERT_EQUAL(actual->frames->size, 10);

  for (int i = 0; i < 10; i++)
  {
    const ParserTestData td = test_data[i];

    const TagV2Frame *frame = get_tag_v2_frame(actual, td.id);
    CU_ASSERT_PTR_NOT_NULL_FATAL(frame);

    const TagV2FrameHeader *header = &frame->header;

    CU_ASSERT_STRING_EQUAL(header->id, td.header_id);
    CU_ASSERT_STRING_EQUAL(frame->body, td.frame_body);
    CU_ASSERT_STRING_EQUAL(frame->unique_id, td.id);
    CU_ASSERT_EQUAL(header->size, td.header_size);
    CU_ASSERT_EQUAL(header->has_zero_byte, td.header_has_zero_byte);
  }

  close_and_remove_test_file();

  delete_tag_v2(actual);
}

void test_with_invalid_files()
{
  fp = open_test_file_in_read_write_mode();

  if (!fp)
  {
    return;
  }

  unsigned char mock[6240] = {0};
  memcpy(mock, id3_v2_tag_mock, 6240);
  mock[2] = 0x34;

  write_data_to_test_file_and_rewind(mock, 6240);

  CU_ASSERT_PTR_NULL(parseMP3(&DEFAULT_MEMMAP, fp));

  CU_ASSERT_PTR_NULL(parseMP3(&DEFAULT_MEMMAP, NULL));

  close_and_remove_test_file();

  fp = open_test_file_in_read_write_mode();

  if (!fp)
  {
    return;
  }

  unsigned char mock2[3] = {0x49, 0x44, 0x33};

  write_data_to_test_file_and_rewind(mock2, 3);

  CU_ASSERT_PTR_NULL(parseMP3(&DEFAULT_MEMMAP, fp));

  close_and_remove_test_file();

  fp = open_test_file_in_read_write_mode();

  if (!fp)
  {
    return;
  }

  unsigned char mock3[2] = {0x49, 0x44};

  write_data_to_test_file_and_rewind(mock3, 2);

  CU_ASSERT_PTR_NULL(parseMP3(&DEFAULT_MEMMAP, fp));

  close_and_remove_test_file();

  fp = open_test_file_in_read_write_mode();

  if (!fp)
  {
    return;
  }

  unsigned char mock4[300] = {0};
  memcpy(mock4, id3_v2_tag_mock, 300);

  write_data_to_test_file_and_rewind(mock4, 300);

  TagV2 *actual = parseMP3(&DEFAULT_MEMMAP, fp);

  CU_ASSERT_EQUAL(actual->frames->size, 2);

  close_and_remove_test_file();

  delete_tag_v2(actual);
}
