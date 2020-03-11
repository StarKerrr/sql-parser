/* Copyright (C) 2020 Vertex, Inc.

> The source file of SQLight Data Base <

This software is free and can be used and modifyied by anyone
under the terms of the GNU Lesser General Public License as
published by the Free Software Foundation, either version 3
of the License, or any later version.

*/

#include "parser.h"

Tokens_st* stack;

/*
 *
 */
void parse(Entity* entity)
{
    ;
}

/* 
 * Creates a new stack for tokens storage
 * @ [in]
 *    -> NULL
 * @ [out]
 *    -> stack, a new stack
 */
Tokens_st* create_tokens_st()
{
    Tokens_st* stack = (Tokens_st*)__smalloc(sizeof(Tokens_st));
    stack->cap = STACK_DEFAULT_CAP;
    stack->size = 0;
    stack->stack = (Entity**)__smalloc(sizeof(Entity) * stack->cap);
}

/*
 *
 */
void _stack_grow(Tokens_st* stack, Entity* token)
{
    if (stack->size >= stack->cap) {
        stack_realloc(stack, stack->cap * 2);
    }
    stack->stack[stack->size++] = token;
}


/*
 *
 */
Entity* stack_pop(Tokens_st* stack)
{
    if (stack->size == 0) {
        _SYSTEM_ERR("Trying to pop from empty stack");
    }
    unsigned int block_size = stack->size - 1;
    size_t memblock = sizeof(int) * block_size;
    memmove(
            &(stack->stack[block_size]),
            &(stack->stack[block_size + 1]),
            memblock
    );
    stack_realloc(stack, stack->cap - 1);
    stack->size--;
}