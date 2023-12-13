#include "main.h"
/**
 * environment - Print the environment variables.
 * Return 0
 */
void print_environment() {
    char **env = environ;
    while (*env != NULL) {
        size_t len = strlen(*env);
        write(STDOUT_FILENO, *env, len);
        write(STDOUT_FILENO, "\n", 1);
        env++;
    }
}