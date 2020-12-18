#include <CUnit/CUnit.h>
#include "mem_test.h"
#include "src/mem/mem.h"

void test_memmap_allocate()
{
  MemMap *mem = new_memmap();

  CU_ASSERT_PTR_NULL(mem->allocate(NULL, "key", 3, 5));
  CU_ASSERT_PTR_NULL(mem->allocate(mem, NULL, 3, 5));

  CU_ASSERT_PTR_NOT_NULL(mem->allocate(mem, "key", 4, sizeof(int)));
  mem->free(mem, "key");

  mem->free(NULL, "key");
  mem->free(mem, NULL);

  delete_memmap(NULL);
  delete_memmap(mem);
}

void test_block_allocation()
{
  MemMap *mem = new_memmap();

  mem->block_allocation_create(NULL);

  for (int j = 0; j < 2; j++)
  {
    mem->block_allocation_create(mem);

    for (int i = 0; i < 40; i++)
    {
      CU_ASSERT_PTR_NOT_NULL(mem->block_allocation_push(mem, 3, 5));
    }
  }

  for (int j = 0; j < 3; j++)
  {
    mem->block_allocation_delete(mem);
  }

  for (int j = 0; j < 2; j++)
  {
    mem->block_allocation_create(mem);

    for (int i = 0; i < 40; i++)
    {
      CU_ASSERT_PTR_NOT_NULL(mem->block_allocation_push(mem, 3, 5));
    }
  }

  mem->block_allocation_delete(mem);

  delete_memmap(mem);

  CU_ASSERT_PTR_NULL(mem->block_allocation_push(NULL, 3, 5));

  mem->block_allocation_delete(NULL);
}
