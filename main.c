#include "interface.h"
#include "tokenizer.h"

int main(int argc, char** argv)
{
    start:
    greeting();
    while (true)
    {
        Statement* buff = read_input();
        if (strcmp(buff->message, ".exit") == 0) {
            printf("~\n");
            close_reader(buff);
            exit(EXIT_SUCCESS);
        } else if (strcmp(buff->message, ".clear") == 0) {
            system("clear");
            goto start;
        } else {
            Keywords* keywords = init_keywords();
            Status_error status = process_input(buff, keywords);
        }
        close_reader(buff);
    }

    return 0;
}