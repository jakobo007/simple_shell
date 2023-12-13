#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

extern char **environ;

#define MAX_INPUT_SIZE 1024


void display_prompt() {
    write(STDOUT_FILENO, "$ ", 2);
    fflush(stdout);
}


void handle_eof() {
    write(STDOUT_FILENO, "Exiting... \n", strlen("Exiting... \n"));
    exit(EXIT_SUCCESS);
}


void print_environment() {
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
            execute_cmd_path(command, args);
        }
    } else {
        wait(NULL);
    }
}


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

            if (args[1] != NULL) {
                if (chdir(args[1]) != 0) {
                    perror("cd");
                }
            } else {
                fprintf(stderr, "cd: missing argument\n");
            }
        } else if (strcmp(args[0], "exit") == 0) {

            exit(EXIT_SUCCESS);
        } else if (strcmp(args[0], "env") == 0) {

            print_environment();
        } else {

            execute_command(args[0], args);
        }
    }
}

int main() {
    char *input = NULL;
    size_t size_of_input = 0;


    signal(SIGTERM, handle_eof);
    signal(SIGINT, display_prompt);

    while (1) {
        display_prompt();

        if (getline(&input, &size_of_input, stdin) == -1) {
            perror("getline");
            exit(EXIT_FAILURE);
        }

        input[strcspn(input, "\n")] = '\0';
        tokenize_and_execute(input);
    }

    free(input);

    return 0;
}
