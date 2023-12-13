#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

extern char **environ;

#define MAX_INPUT_SIZE 1024

void new_line() {
    write(STDOUT_FILENO, "\n$ ", 3);
    fflush(stdout);
}

void end_of_file() {
    write(STDOUT_FILENO, "Exiting... \n", strlen("Exiting... \n"));
    _exit(EXIT_SUCCESS);
}

void environment() {
    char **env = environ;
    while (*env != NULL) {
        size_t len = strlen(*env);
        write(STDOUT_FILENO, *env, len);
        write(STDOUT_FILENO, "\n", 1);
        env++;
    }
}

void execute_cmd_path(char *cmd, char *args[]) {
    char *path = getenv("PATH");
    char *copy_path = strdup(path);
    char *dir = strtok(copy_path, ":");
    if (path == NULL) {
        write(STDERR_FILENO, "PATH environment variable not set\n", strlen("PATH environment variable not set\n"));
        _exit(EXIT_FAILURE);
    }

    while (dir != NULL) {
        char *full_path = malloc(strlen(dir) + strlen(cmd) + 2);
        sprintf(full_path, "%s/%s", dir, cmd);

        if (access(full_path, X_OK) == 0) {
            execve(full_path, args, environ);
            perror("execve");
            _exit(EXIT_FAILURE);
        }

        free(full_path);
        dir = strtok(NULL, ":");
    }

    write(STDERR_FILENO, cmd, strlen(cmd));
    write(STDERR_FILENO, ": command not found\n", strlen(": command not found\n"));
    free(copy_path);
    _exit(EXIT_FAILURE);
}

void execute_command(char *command, char *args[]) {
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        _exit(EXIT_FAILURE);
    } else if (pid == 0) {
        if (strchr(command, '/') != NULL) {
            execve(command, args, environ);
            perror(args[0]);
            _exit(EXIT_FAILURE);
        } else {
            execute_cmd_path(command, args);
        }
    } else {
        wait(NULL);
    }
}

void gettoken(char *input) {
    const char delimiter[] = " \t\n";
    char *token = strtok(input, delimiter);
    char *args[MAX_INPUT_SIZE];
    int i = 0;

    while (token != NULL) {
        if (i >= MAX_INPUT_SIZE - 1) {
            write(STDERR_FILENO, "Too many arguments\n", strlen("Too many arguments\n"));
            return;
        }
        args[i++] = token;
        token = strtok(NULL, delimiter);
    }

    args[i] = NULL;

    if (i > 0) {
        if (strcmp(args[0], "cd") == 0) {
            if (args[1] != NULL) {
                if (chdir(args[1]) != 0) {
                    perror("cd");
                }
            } else {
                write(STDERR_FILENO, "cd: missing argument\n", strlen("cd: missing argument\n"));
            }
        } else if (strcmp(args[0], "exit") == 0) {
            _exit(EXIT_SUCCESS);
        } else if (strcmp(args[0], "env") == 0) {
            environment();
        } else {
            execute_command(args[0], args);
        }
    }
}

int main() {
    char *input = NULL;
    size_t size_of_input = 0;

    signal(SIGTERM, end_of_file);
    signal(SIGINT, new_line);

    while (1) {
        new_line();

        if (getline(&input, &size_of_input, stdin) == -1) {
            write(STDERR_FILENO, "getline error\n", strlen("getline error\n"));
            _exit(EXIT_FAILURE);
        }

        input[strcspn(input, "\n")] = '\0';
        gettoken(input);
    }

    free(input);
    return 0;
}
