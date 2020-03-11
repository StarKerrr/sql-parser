#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <math.h>

#include "dberrors.h"

#ifndef _SQLIGHT_LEXIS_H
#define _SQLIGHT_LEXIS_H

#define str_buff_push(buffer, element) ( (buffer) ? _buff_grow(buffer, element) : 0 )
#define buff_realloc(buffer) ( (buffer) ? realloc(buffer->data, 2 * buffer->cap) : 0 ); buffer->cap *= 2

#define STR_VIEW(x) to_uppercase(#x)
#define ARR_LENGTH(a) ( sizeof(a) / sizeof(*a) )
  
#define ERR_MESS_MAXLEN 128


/* Custom memory allocation func */
void* __smalloc(size_t);


// Working with string buffers //

/* Structure for strings */
typedef struct string_buff {
    size_t len;
    size_t cap;
    char* data;
} String_buffer;


/* Creates a new string buffer */
String_buffer* new_string_buff();

/* Creates a new string buffer from a string */
String_buffer* new_string_buffer_from(char*);

/* Pushing an element in the buffer */
void _buff_grow(String_buffer*, char);


////////////////////////////////


// Working with SQLight Keywords set //

/* Structure of keywords container */
typedef struct keywords {
    char** list;
    size_t capacity;
    size_t size;
} Keywords;


/* Creates a new keywords container */
Keywords* create_keywords();

/* Keywords initialization */
Keywords* init_keywords();

/* Pushing a new keyword in keywords container */
void update_keywords(Keywords*, char*);


///////////////////////////////////////


/* Structure of statement (query) */
typedef struct buffer {
    char* message;
    size_t size;
    size_t length;
} Statement;


/* Types of a single entity in a query */
typedef enum entity_type {
    ENTITY_KEYWORD,
    ENTITY_WORD,
    ENTITY_INT,
    ENTITY_FLOAT,
    ENTITY_SPEC_SYMBOL,
    ENTITY_COMPARE_BIN,
    ENTITY_COMPARE_UN,
    ENTITY_NONE
} entity_t;

/* Structure of token (part of query) */
typedef struct entity {
    entity_t type;
    long int length;
    char* start;
    char* end;
    Status_error status;

    /* data that can be stored here  */
    union {
        /* represents the set of keywords and other words*/
        char* string_data;
        
        /* represents float data */
        float float_data;
        
        /* represents all integer numbers */
        uint64_t int_data;

        /* represents a single character from the set of special symbols */
        char symbol_data;

        /* represents comparison binary operator */
        char* comp_symbol_bin;

        /* represents comparison unary operator */
        char comp_symbol_un;
    };
} Entity;


/* Creates a new entity*/
Entity* new_entity();

/* Checks if entity type is KEYWORD */
bool is_keyword(Keywords*, char*);

/* Getting a given entity type */
entity_t _get_entity_type(Entity*);

/* Converts a given string to upper case */
char* to_uppercase(char*);

/* Converts char to digit */
uint8_t char_to_digit[256];



#endif  // _SQLIGHT_LEXIS_H