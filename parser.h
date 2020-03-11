#include "lexis.h"

#ifndef _SQLIGHT_PARSER_H
#define _SQLIGHT_PARSER_H

#define stack_push(t_stack, element) ( (t_stack) ? _stack_grow(t_stack, element) )
#define stack_realloc(t_stack, bytes) ( (t_stack) ? realloc(t_stack->stack, bytes) : 0 ); t_stack->cap = bytes

#define STACK_DEFAULT_CAP 2

/* Parsing a given token */
void parse(Entity*);


// Work with a stack of tokens //

/* Structure for tokens storage, it's implemented as a stack */
typedef struct tokens_stack
{
    Entity** stack;
    size_t cap;
    size_t size;
} Tokens_st;


/* Creates a new stack for tokens storage */
Tokens_st* create_tokens_st();

/* Appending a new entity to the stack */
void _stack_grow(Tokens_st*, Entity*);

/* Getting and remove the last element of the stack */
Entity* stack_pop(Tokens_st*);



/////////////////////////////////



#endif // _SQLIGHT_PARSER_H