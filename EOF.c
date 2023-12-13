#include "main.h"
void end_of_file() {
        write(STDOUT_FILENO, "Exiting... \n", strlen("Exiting... \n"));
        exit(EXIT_SUCCESS);
}
