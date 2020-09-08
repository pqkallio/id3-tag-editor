#include <malloc.h>
#include "stack.h"

Stack* new_stack(unsigned long size)
{
    Stack* stack = calloc(1, sizeof(Stack));
    void** arr = calloc(size + 1, sizeof(void*));

    stack->stack = arr;
    stack->max = size + 1;

    stack->stack[0] = &BOTTOM;

    return stack;
}

void delete_stack(Stack* stack)
{
    if (!stack) {
        return;
    }

    if (stack->stack) {
        free(stack->stack);
    }

    free(stack);
}

void clear_stack(Stack* stack)
{
    if (!stack) return;

    stack->idx = 0;
}

unsigned int push_to_stack(Stack* stack, void* item)
{
    if (!item || !stack || !stack->stack || stack->idx >= stack->max - 1) {
        return 0;
    }

    stack->idx++;

    stack->stack[stack->idx] = item;

    return 1;
}

void* pop_stack(Stack* stack)
{
    if (!stack || !stack->stack || stack->idx == 0) {
        return NULL;
    }

    void* item = stack->stack[stack->idx];

    stack->idx--;

    return item;
}

void* peek_stack(Stack* stack)
{
    void* item = pop_stack(stack);

    if (item == NULL) {
        return NULL;
    }

    push_to_stack(stack, item);

    return item;
}
