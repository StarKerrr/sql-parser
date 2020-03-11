/* Copyright (C) 2020 Vertex, Inc.

> The source file of SQLight Data Base <

This software is free and can be used and modifyied by anyone
under the terms of the GNU Lesser General Public License as
published by the Free Software Foundation, either version 3
of the License, or any later version.

*/

#include "lexis.h"


/*
 * Allocates a given number of bytes
 * @ [in]
 *    -> bytes, the number of bytes that should be allocated
 * @ [out]
 *    -> memory_block, block of memory that was allocated
 */
void* __smalloc(size_t bytes)
{
    void* memory_block = malloc(bytes);
    if (!memory_block) {
        free(memory_block);
        _SYSTEM_ERR("Can't allocate memory");
    }

    return memory_block;
}


/*
 * Creates a new string buffer
 * @ [in]
 *    -> NULL
 * @ [out]
 *    -> new_buff, new created string buffer
 */
String_buffer* new_string_buff()
{
    String_buffer* new_buff = (String_buffer*)__smalloc(sizeof(String_buffer));
    new_buff->len = 0;
    new_buff->cap = 2;
    new_buff->data = (char*)__smalloc(sizeof(char) * new_buff->cap);

    return new_buff;
}


/*
 * Making the string buffer from a given string
 * @ [in]
 *    -> string, string from which a new string buffer should be built
 * @ [out]
 *    -> buffer, a new string buffer created from the string
 */
String_buffer* new_string_buffer_from(char* string)
{
    String_buffer* buffer = new_string_buff();
    size_t length = ARR_LENGTH(string);

    for (int i = 0; i < length; i++) {
        str_buff_push(buffer, string[i]);
    }
    str_buff_push(buffer, '\0');

    return buffer;
}


/*
 * Pushing an element into the buffer
 * @ [in]
 *    -> buffer, buffer that should grow
 *    -> element, element that should be appended to the buffer
 * @ [out]
 *    -> NULL
 */
void _buff_grow(String_buffer* buffer, char element)
{
    if (buffer->len >= buffer->cap) {
        buff_realloc(buffer);
    }
    buffer->data[buffer->len++] = element;
}


/* 
 * Creates a new keywords container
 * @ [in]
 *    -> NULL
 * @ [out]
 *    -> keywords, container of keywords
 */
Keywords* create_keywords()
{
    Keywords* keywords = (Keywords*)__smalloc(sizeof(Keywords));
    keywords->capacity = 50;
    keywords->size = 0;
    keywords->list = (char**)__smalloc(sizeof(char*) * keywords->capacity);

    return keywords;
}

/* SQLight Keywords initialization */
Keywords* init_keywords() {
    Keywords* KEYWORDS = create_keywords();

    update_keywords(KEYWORDS, STR_VIEW(create));
    update_keywords(KEYWORDS, STR_VIEW(show));
    update_keywords(KEYWORDS, STR_VIEW(use));
    update_keywords(KEYWORDS, STR_VIEW(describe));
    update_keywords(KEYWORDS, STR_VIEW(database));
    update_keywords(KEYWORDS, STR_VIEW(select));
    update_keywords(KEYWORDS, STR_VIEW(from));
    update_keywords(KEYWORDS, STR_VIEW(where));
    update_keywords(KEYWORDS, STR_VIEW(having));
    update_keywords(KEYWORDS, STR_VIEW(between));
    update_keywords(KEYWORDS, STR_VIEW(as));
    update_keywords(KEYWORDS, STR_VIEW(limit));
    update_keywords(KEYWORDS, STR_VIEW(orderby));
    update_keywords(KEYWORDS, STR_VIEW(groupby));
    update_keywords(KEYWORDS, STR_VIEW(distinct));
    update_keywords(KEYWORDS, STR_VIEW(unique));
    update_keywords(KEYWORDS, STR_VIEW(desc));
    update_keywords(KEYWORDS, STR_VIEW(asc));
    update_keywords(KEYWORDS, STR_VIEW(like));
    update_keywords(KEYWORDS, STR_VIEW(concat));
    update_keywords(KEYWORDS, STR_VIEW(insertinto));
    update_keywords(KEYWORDS, STR_VIEW(values));
    update_keywords(KEYWORDS, STR_VIEW(update));
    update_keywords(KEYWORDS, STR_VIEW(set));
    update_keywords(KEYWORDS, STR_VIEW(drop));
    update_keywords(KEYWORDS, STR_VIEW(delete));
    update_keywords(KEYWORDS, STR_VIEW(notnull));
    update_keywords(KEYWORDS, STR_VIEW(isnull));
    update_keywords(KEYWORDS, STR_VIEW(isnotnull));
    update_keywords(KEYWORDS, STR_VIEW(in));
    update_keywords(KEYWORDS, STR_VIEW(notin));
    update_keywords(KEYWORDS, STR_VIEW(all));
    update_keywords(KEYWORDS, STR_VIEW(any));
    update_keywords(KEYWORDS, STR_VIEW(and));
    update_keywords(KEYWORDS, STR_VIEW(or));
    update_keywords(KEYWORDS, STR_VIEW(xor));
    update_keywords(KEYWORDS, STR_VIEW(upper));
    update_keywords(KEYWORDS, STR_VIEW(lower));
    update_keywords(KEYWORDS, STR_VIEW(sqrt));
    update_keywords(KEYWORDS, STR_VIEW(sum));
    update_keywords(KEYWORDS, STR_VIEW(min));
    update_keywords(KEYWORDS, STR_VIEW(max));

    return KEYWORDS;
}


/*
 * Pushing a given keyword to the container of all keywords
 * @ [in]
 *    -> keywords, Keywords object which should be updated
 *    -> new_keyword, keyword that should be pushed
 * @ [out]
 *    -> NULL
 */
void update_keywords(Keywords* keywords, char* new_keyword)
{
    if (keywords->size >= keywords->capacity) {
        keywords->list = realloc(keywords->list, keywords->capacity * 2);
    }
    keywords->list[keywords->size++] = new_keyword;
}


/*
 * Creates a new entity
 * @ [in]
 *    -> NULL
 * @ [out]
 *    -> entity, new entity for further parsing
 */
Entity* new_entity()
{
    Entity* entity = (Entity*)__smalloc(sizeof(Entity));
    entity->type = ENTITY_NONE;
    entity->length = 0;

    return entity;
}


/*
 * Returns true if a given entity has type 'KEYWORD'
 * @ [in]
 *    -> entity, entity type of which should be checked
 * @ [out]
 *    -> bool, the result of checking
 */
bool is_keyword(Keywords* keywords, char* word)
{   
    for (int i = 0; i < keywords->size; i++) {
        if (strcmp(keywords->list[i], word) == 0) {
            return true;
        }
    }
    return false;
}


/*
 * Returns type of a given entity
 * @ [in]
 *    -> entity, entity type of which should be returned
 * @ [out]
 *    -> type, type of a given entity
 */
entity_t _get_entity_type(Entity* entity)
{
    return entity->type;
}


/*
 * Converts a given expression to uppercase
 * @ [in]
 *    -> expression, an expression that should be cnoverted to uppercase
 * @ [out]
 *    -> expression, converted expression
 */
char* to_uppercase(char* expression)
{
    String_buffer* buffer = new_string_buff();
    while (*expression) {
        char token = *expression;
        if (isalpha(*expression))
            token = toupper(token);
        str_buff_push(buffer, token);
        expression++;
    }
    str_buff_push(buffer, '\0');

    return buffer->data;
}


/* Converts char to digit */
uint8_t char_to_digit[256] = {
    ['0'] = 0,
    ['1'] = 1,
    ['2'] = 2,
    ['3'] = 3,
    ['4'] = 4,
    ['5'] = 5,
    ['6'] = 6,
    ['7'] = 7,
    ['8'] = 8,
    ['9'] = 9
};