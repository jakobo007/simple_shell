#include "main.h"
/**
 * new_line
 **/
void new_line() {
        write(STDOUT_FILENO, "\n$ ", 4);
        fflush(stdout);
}

