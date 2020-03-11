/* Copyright (C) 2020 Vertex, Inc.

> The source file of SQLight Data Base <

This software is free and can be used and modifyied by anyone
under the terms of the GNU Lesser General Public License as
published by the Free Software Foundation, either version 3
of the License, or any later version.

*/

#include "tokenizer.h"
#include "parser.h"


/*
 * Entry point for the statement processing
 * @ [in]
 *    -> buffer, the input buffer object with a statement
 *    -> keywords, keywords of SQLight syntax, should define type of tokens
 * @ [out]
 *    -> NULL
 */
Status_error tokenize(Statement* buffer, Keywords* keywords)
{
    char* statement = to_uppercase(buffer->message);
    while (*statement != '\0')
    {
        Entity* token = _next_token(&statement, keywords);
        if (token->status == STATUS_SUCCESS) {
            print_token(token);
            parse(token);
        } else {
            return STATUS_FAILURE;
        }
    }
}


/*
 * Tokens pretty printing
 * @ [in]
 *    -> entity, token information of which should be printed
 * @ [out]
 *    -> NULL
 */
void print_token(Entity* entity)
{
    switch (entity->type) {
        case ENTITY_INT: printf("INT_TOKEN: %lu\n", entity->int_data); break;
        case ENTITY_FLOAT: printf("FLOAT_TOKEN: %f\n", entity->float_data); break;
        case ENTITY_KEYWORD: printf("KEYWORD_TOKEN: %s\n", entity->string_data); break;
        case ENTITY_WORD: printf("WORD_TOKEN: %s\n", entity->string_data); break;
        case ENTITY_SPEC_SYMBOL: printf("SYMBOL_TOKEN: %c\n", entity->symbol_data); break;
        case ENTITY_COMPARE_BIN: printf("COMPARISON_BINARY_TOKEN: %s\n", entity->comp_symbol_bin); break;
        case ENTITY_COMPARE_UN: printf("COMPARISON_UNARY_TOKEN: %d\n", entity->comp_symbol_un); break;
        case ENTITY_NONE: printf("\n");
    }
}


/*
 * Getting next token from the statement
 * @ [in]
 *    -> statement, the statement that should be tokenized
 *    -> keywords, keywords which should define the type of token
 * @ [out]
 *    -> entity, next token of a given statement
 */
Entity* _next_token(char** statement, Keywords* keywords)
{   
    Entity* entity = new_entity();
    char* ERR_MESSAGE = (char*)__smalloc(ERR_MESS_MAXLEN * sizeof(char));

    start:
    entity->start = *statement;
    switch (**statement) {
    case ' ': case '\n': case '\b': case '\t': case '\v':
        while (isspace(**statement)) {
            (*statement)++;
        }
        goto start;
    case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': case 'M':
    case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
        entity->string_data = _parse_string_token(statement);
        if (is_keyword(keywords, entity->string_data)) {
            entity->type = ENTITY_KEYWORD;
        } else {
            entity->type = ENTITY_WORD;
        }
        entity->status = STATUS_SUCCESS;
        break;
    case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
        while (isdigit(**statement)) {
            (*statement)++;
        }
        char c = **statement;
        *statement = entity->start;
        if (c == '.') {
            float float_value = _parse_float_token(statement);
            entity->float_data = float_value;
            entity->type = ENTITY_FLOAT;
        } else {
            uint64_t int_value = _parse_integer_token(statement);
            if (int_value != -1) {
                entity->int_data = int_value;
                entity->type = ENTITY_INT;
            } else {
                entity->status = STATUS_FAILURE;
                return entity;
            }
        }
        entity->status = STATUS_SUCCESS;
        break;
    case '*': case ',': case '$': case '#': case '%': case '_': case '^': case '[': case ']':
    case '{': case '}': case '(': case ')':  case '-': case '.': case ';':
        if (**statement == ';') {
            char* state_c = *statement;
            state_c++;
            if (*state_c == '\0') { ; } 
            else { goto invalid_token_error; }
        }

        entity->symbol_data = **statement;
        entity->type = ENTITY_SPEC_SYMBOL;
        entity->status = STATUS_SUCCESS;
        (*statement)++;
        break;

    /* Actual set of operators: {'=', '!=', '<>', '>', '>=', '<', '<='} */
    String_buffer* operator_buff = new_string_buff();
    case '=': case '!': case '>': case '<':
        str_buff_push(operator_buff, **statement);
        char ch = **statement;
        (*statement)++;
        if (ispunct(**statement)) {
            str_buff_push(operator_buff, ch);
            entity->type = ENTITY_COMPARE_BIN;
            entity->comp_symbol_bin = operator_buff->data;
            (*statement)++;
        } else {
            entity->type = ENTITY_COMPARE_UN;
            entity->comp_symbol_un = ch;
        }
        entity->status = STATUS_SUCCESS;
        break;

    invalid_token_error:
    default:
        sprintf(ERR_MESSAGE, "%s: '%c'", "Invalid token was found", **statement);
        _PARSE_ERR(ERR_MESSAGE);
        entity->status = STATUS_FAILURE;
        return entity;
    }

    entity->end = *statement;
    entity->length = entity->end - entity->start;
    if (entity->length >= TOKEN_MAXLEN) {
        _VALUE_ERR("Maximum token length exceeded");
        entity->status = STATUS_FAILURE;
    }

    return entity;
}


/*
 * Parses string token from the statement
 * @ [in]
 *    -> statement, expression that should be tokenized
 * @ [out]
 *    -> string_data, string data of buffer containig parsed token
 */
char* _parse_string_token(char** statement)
{   
    String_buffer* string_buffer = new_string_buff();

    while (isalpha(**statement)) {
        str_buff_push(string_buffer, **statement);
        (*statement)++;
    }
    str_buff_push(string_buffer, '\0');

    return string_buffer->data;
}


/*
 * Parses integer from the statement
 * @ [in]
 *    -> statement, expression that should be tokenized
 * @ [out]
 *    -> int_value, parsed integer value
 */
uint64_t _parse_integer_token(char** statement)
{
    long int int_value = 0;
    while (isdigit(**statement)) {
        if (int_value >= (long)1 << 32) {
            _VALUE_ERR("Integer literal overflow");
            return -1;
        }
        int_value = int_value * 10 + char_to_digit[**statement];
        (*statement)++;
    }

    return int_value;    
}


/*
 * Parses float from the statement
 * @ [in]
 *    -> statement, expression that should be tokenized
 * @ [out]
 *    -> float_value, parsed float value
 */
float _parse_float_token(char** statement)
{
    float float_value;
    const char* start = *statement;
    while (isdigit(**statement)) (*statement)++;
    if (**statement == '.') (*statement)++;
    while (isdigit(**statement)) (*statement)++;

    float_value = strtod(start, NULL);
    if (float_value == HUGE_VAL) {
        _VALUE_ERR("Float literal overflow");
    }

    return float_value;
}