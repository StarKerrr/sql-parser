/* Copyright (C) 2020 Vertex, Inc.

> The source file of SQLight Data Base <

This software is free and can be used and modifyied by anyone
under the terms of the GNU Lesser General Public License as
published by the Free Software Foundation, either version 3
of the License, or any later version.

*/

#include "dberrors.h"


/* System errors like memory allocation error */
void __system_error(const char* message)
{
    printf("SQL System Error: %s\n", message);
    exit(EXIT_FAILURE);
}

/* Parsing error like missing some keyword */
void __parse_error(const char* message)
{
    printf("SQLight Parsing Error: %s\n", message);
}
 
/* Value errors like wrong VALUES numeric value */
void __value_error(const char* message)
{
    printf("SQLight Value Error: %s\n", message);
}