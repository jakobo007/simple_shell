#include "main.h"
/**
 * new_line - Displays a new line and shell prompt.
 */
void new_line() {
write(STDOUT_FILENO, "\n$ ", 4);
fflush(stdout);
}