#include <CUnit/CUnit.h>
#include "stack_test.h"
#include "src/dstructs/stack.h"

void test_stack()
{
    Stack* stack = new_stack(2);

    CU_ASSERT_PTR_NOT_NULL_FATAL(stack);
    CU_ASSERT_EQUAL_FATAL(stack->idx, 0);
    CU_ASSERT_EQUAL_FATAL(stack->max, 3);

    int baba[3];

    CU_ASSERT_PTR_NULL_FATAL(pop_stack(stack));

    CU_ASSERT_EQUAL_FATAL(push_to_stack(stack, &baba[0]), 1);
    CU_ASSERT_EQUAL_FATAL(push_to_stack(stack, &baba[1]), 1);
    CU_ASSERT_EQUAL_FATAL(push_to_stack(stack, &baba[2]), 0);

    CU_ASSERT_PTR_EQUAL_FATAL(peek_stack(stack), &baba[1]);
    CU_ASSERT_PTR_EQUAL_FATAL(pop_stack(stack), &baba[1]);
    CU_ASSERT_PTR_EQUAL_FATAL(pop_stack(stack), &baba[0]);

    CU_ASSERT_PTR_NULL_FATAL(pop_stack(stack));
    CU_ASSERT_PTR_NULL_FATAL(peek_stack(stack));

    push_to_stack(stack, &baba[0]);
    push_to_stack(stack, &baba[1]);

    CU_ASSERT_EQUAL_FATAL(stack->idx, 2);
    clear_stack(stack);
    CU_ASSERT_EQUAL_FATAL(stack->idx, 0);

    delete_stack(stack);
}
