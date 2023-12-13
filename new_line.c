#include "main.h"
/**
 * new_line - Print a new line with a prompt.
 */
void new_line() {
write(STDOUT_FILENO, "\n$ ", 4);
fflush(stdout);
}
