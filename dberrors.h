#include <stdio.h>
#include <stdlib.h>

#ifndef _SQLIGHT_ERRORS_H
#define _SQLIGHT_ERRORS_H

void __system_error(const char*);
void __parse_error(const char*);
void __value_error(const char*);


#define _SYSTEM_ERR(mess) __system_error(mess)
#define _PARSE_ERR(mess) __parse_error(mess)
#define _VALUE_ERR(mess) __value_error(mess)


typedef enum errors {
    STATUS_FAILURE,
    STATUS_SUCCESS,
} Status_error;



#endif // _SQLIGHT_ERRORS_H