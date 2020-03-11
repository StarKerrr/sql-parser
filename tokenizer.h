#include "lexis.h"

#ifndef _SQLIGHT_TOKENIZER_H
#define _SQLIGHT_TOKENIZER_H

#define TOKEN_MAXLEN 128

Status_error tokenize(Statement*, Keywords*);
void print_token(Entity*);
Entity* _next_token(char**, Keywords*);
char* _parse_string_token(char**);
uint64_t _parse_integer_token(char**);
float _parse_float_token(char**);



#endif // _SQLIGHT_TOKENIZER_H