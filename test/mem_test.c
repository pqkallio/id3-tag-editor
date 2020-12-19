#include <CUnit/CUnit.h>
#include "mem_test.h"
#include "src/mem/mem.h"

void test_memmap_allocate()
{
  MemMap *mem = new_memmap();

  CU_ASSERT_PTR_NULL(mem->allocate(NULL, 3, 5));

  CU_ASSERT_PTR_NOT_NULL(mem->allocate(mem, 4, sizeof(int)));
  mem->free(mem, "key");

  mem->free(NULL, "key");
  mem->free(mem, NULL);

  delete_memmap(NULL);
  delete_memmap(mem);
}
