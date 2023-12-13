#include "main.h"
/**
 * execute_CMD_PATH - Execute a command by searching in the PATH.
 * @cmd: The command to execute.
 * @args: Arguments for the command.
 * Return: 0
 */
void execute_cmd_path(char *cmd, char *args[]) {
    char *path = getenv("PATH");
    char *copy_path = strdup(path);
    char *dir = strtok(copy_path, ":");

    if (path == NULL) {
        fprintf(stderr, "PATH environment variable not set\n");
        exit(EXIT_FAILURE);
    }

    while (dir != NULL) {
        char *full_path = malloc(strlen(dir) + strlen(cmd) + 2);
        sprintf(full_path, "%s/%s", dir, cmd);

        if (access(full_path, X_OK) == 0) {
            execve(full_path, args, environ);
            perror("execve");
            exit(EXIT_FAILURE);
        }
        free(full_path);
        dir = strtok(NULL, ":");
    }

    fprintf(stderr, "%s: command not found\n", args[0]);
    free(copy_path);
    exit(EXIT_FAILURE);
}