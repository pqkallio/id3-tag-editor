#ifndef _DSTRUCTS_STACK_H
#define _DSTRUCTS_STACK_H

static const char STACK_BOTTOM_;

typedef struct _stack
{
    unsigned long max;
    unsigned long idx;
    void **stack;
} Stack;

Stack *new_stack(unsigned long size);
void delete_stack(Stack *stack);
void clear_stack(Stack *stack);
unsigned int push_to_stack(Stack *stack, const void *item);
void *pop_stack(Stack *stack);
void *peek_stack(Stack *stack);

#endif /* _DSTRUCTS_STACK_H */
