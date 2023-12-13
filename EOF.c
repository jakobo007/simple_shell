#include "main.h"
/**
 * end_of_file - Displays a message and exits the shell.
 */
void end_of_file(void) {
write(STDOUT_FILENO, "Exiting... \n", strlen("Exiting... \n"));
exit(EXIT_SUCCESS);
}
