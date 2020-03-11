#include "lexis.h"
#include "tokenizer.h"


# ifndef _SQLIGHT_INTERFACE_H
#define _SQLIGHT_INTERFACE_H


Statement* new_statement();
struct tm* local_time();
void greeting();
Statement* read_input();
Status_error process_input(Statement*, Keywords*);
void close_reader();



#endif  // _SQLIGHT_INTERFACE_H