#include "main.h"
/**
 * gettoken - Tokenize input and execute the appropriate action.
 * @input: The input string to tokenize.
 * Return 0
 */
void tokenize_and_execute(char *input) {
    const char delimiter[] = " \t\n";
    char *token = strtok(input, delimiter);
    char *args[MAX_INPUT_SIZE];
    int i = 0;

    while (token != NULL) {
        if (i >= MAX_INPUT_SIZE - 1) {
            fprintf(stderr, "Too many arguments\n");
        }
        args[i++] = token;
        token = strtok(NULL, delimiter);
    }

    args[i] = NULL;

    if (i > 0) {
        if (strcmp(args[0], "cd") == 0) {
            // Handle "cd" command
            if (args[1] != NULL) {
                if (chdir(args[1]) != 0) {
                    perror("cd");
                }
            } else {
                fprintf(stderr, "cd: missing argument\n");
            }
        } else if (strcmp(args[0], "exit") == 0) {
            // Handle "exit" command
            exit(EXIT_SUCCESS);
        } else if (strcmp(args[0], "env") == 0) {
            // Handle "env" command
            print_environment();
        } else {
            // Execute the command
            execute_command(args[0], args);
        }
    }
}