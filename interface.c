/* Copyright (C) 2020 Vertex, Inc.

> The source file of SQLight Data Base <

This software is free and can be used and modifyied by anyone
under the terms of the GNU Lesser General Public License as
published by the Free Software Foundation, either version 3
of the License, or any later version.

*/

#include "interface.h"


/*
 * Creates a new buffer for input command
 * @ [in]
 *    -> NULL
 * @ [out]
 *    -> buffer, new buffer for input data recording
 */
Statement* new_statement()
{
    Statement* buffer = (Statement*)__smalloc(sizeof(Statement));
    buffer->size = 0;
    buffer->length = 0;
    buffer->message = (char*)__smalloc(sizeof(char));

    return buffer;
}


/*
 * Getting a local time tm* structure
 * @ [in]
 *    -> NULL
 * @ [out]
 *    -> timeinfo, a struct tm object representing datetime
 */
struct tm* local_time()
{
    time_t rawtime;
    struct tm * timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime );
	return timeinfo;
}


/*
 * Greeting a user every time when the SQLight is invoked
 * @ [in]
 *    -> NULL
 * @ [out]
 *    -> NULL
 */
void greeting()
{
    struct tm* cur_time = local_time();
    printf("SQLight version 1.0 %s", asctime(cur_time));
    printf("Enter \".help\" for usage hints.\nConnected to a transient in-memory database.\n");
    printf("Use \".open FILENAME\" to reopen on a persistent database.\n");
}


/*
 * Getting the input from a user
 * @ [in]
 *    -> NULL
 * @ [out]
 *    -> buffer, buffer with input data
 */
Statement* read_input()
{
    printf("sqlight> ");

    Statement* buffer = new_statement();
    size_t bytes_read = getline(&(buffer->message), &(buffer->size), stdin);
    buffer->length = bytes_read - 1;
    buffer->message[bytes_read - 1] = '\0';

    return buffer;
}


/*
 * Processing the input
 * @ [in]
 *    > statement, the statement that should be processed
 *    > keywords, the keywords which are included in the SQLight lexis
 * @ [out]
 *    > status, status for error detection
 */
Status_error process_input(Statement* statement, Keywords* keywords)
{
    Status_error status = tokenize(statement, keywords);
    return status;
}


/*
 * Freeing a buffer
 * @ [in]
 *    -> buffer, buffer that should be closed
 * @ [out]
 *    -> NULL
 */
void close_reader(Statement* buffer)
{
    free(buffer->message);
    free(buffer);
}