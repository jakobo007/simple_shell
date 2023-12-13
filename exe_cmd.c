#include "main.h"
/**
 * execute_command - Execute a command using fork and execve.
 * @command: The command to execute.
 * @args: Arguments for the command.
 * Return 0
 */
void execute_command(char *command, char *args[]) {
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        if (strchr(command, '/') != NULL) {
            execve(command, args, environ);
            perror(args[0]);
            exit(EXIT_FAILURE);
        } else {
                execute_CMD_PATH(command, args);
         }
    } else {
        wait(NULL);
    }

}
