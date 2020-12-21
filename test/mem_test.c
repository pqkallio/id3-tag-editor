#include <CUnit/CUnit.h>
#include "mem_test.h"
#include "src/mem/mem.h"

void test_memmap_allocate()
{
  MemMap *mem = new_memmap();

  CU_ASSERT_PTR_NULL(mem->allocate(NULL, 3, 5));

  void *a = mem->allocate(mem, 4, sizeof(int));
  void *b = mem->allocate(mem, 10, sizeof(char));

  CU_ASSERT_PTR_NOT_NULL(a);
  CU_ASSERT_PTR_NOT_NULL(b);
  CU_ASSERT_EQUAL(mem->free(mem, "key"), MEME_INVALID_FREE);

  CU_ASSERT_EQUAL(mem->free(NULL, "key"), MEME_MEMMAP_OR_ITEM_NOT_PROVIDED);
  CU_ASSERT_EQUAL(mem->free(mem, NULL), MEME_MEMMAP_OR_ITEM_NOT_PROVIDED);

  CU_ASSERT_EQUAL(mem->free(mem, a), MEME_SUCCESS);

  mem->clear(NULL);
  mem->clear(mem);

  b = mem->allocate(mem, 10, sizeof(char));

  delete_memmap(NULL);
  delete_memmap(mem);
}
