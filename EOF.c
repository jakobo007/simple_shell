#include "main.h"
/**
 * end_of_file - Print an exit message and terminate the program.
 * Return 0
 */
void end_of_file() {
    write(STDOUT_FILENO, "Exitingu... \n", strlen("Exiting... \n"));
    exit(EXIT_SUCCESS);
}

