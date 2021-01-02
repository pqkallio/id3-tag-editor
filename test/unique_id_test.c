#include <CUnit/CUnit.h>
#include <stdlib.h>
#include "unique_id_test.h"
#include "src/util/unique_id.h"

void test_unique_id_with_length_0()
{
  srand(42);

  char id[] = {0, 0, 0, 0, 0, 0, 0};

  generate_unique_id(id, 0);

  CU_ASSERT_STRING_EQUAL(id, "");
}

void test_unique_id_with_length_2()
{
  srand(42);

  char id[] = {0, 0, 0, 0, 0, 0, 0};

  generate_unique_id(id, 2);

  CU_ASSERT_STRING_EQUAL(id, "51");
}

void test_unique_id_with_length_4()
{
  srand(42);

  char id[] = {0, 0, 0, 0, 0, 0, 0};

  generate_unique_id(id, 4);

  CU_ASSERT_STRING_EQUAL(id, "510F");
}

void test_unique_id_with_length_5()
{
  srand(42);

  char id[] = {0, 0, 0, 0, 0, 0, 0};

  generate_unique_id(id, 5);

  CU_ASSERT_STRING_EQUAL(id, "");
}

void test_unique_id_with_length_6()
{
  srand(42);

  char id[] = {0, 0, 0, 0, 0, 0, 0};

  generate_unique_id(id, 6);

  CU_ASSERT_STRING_EQUAL(id, "510FB0");
}

void test_unique_id_with_length_32()
{
  srand(42);

  char id[33] = {0};

  generate_unique_id(id, 32);

  CU_ASSERT_STRING_EQUAL(id, "510FB07ECADF79C34108BD62A245AB11");
}

void test_unique_id_with_length_254()
{
  srand(42);

  char id[255] = {0};

  generate_unique_id(id, 254);

  CU_ASSERT_STRING_EQUAL(id, "510FB07ECADF79C34108BD62A245AB117F306CEA84844A30B1B390B8A165CBF2747CF0BEDB6A831E73C0009506ABA785DC93EFE0193A904A6EA0038F064F82FA4BF2B928DCBC4650FCC56603718D09CD21F82EB9B3BE0422DE87B1E556B360A1A699C983D68F54D3553AD747C7E0946859C3230D02A6AEE1AD6047049327A6");
}
