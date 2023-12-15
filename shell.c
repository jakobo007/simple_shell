#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_INPUT_SIZE 1024
#define MAX_TOKENS 64
extern char **environ;
void display_prompt() {
    write(STDOUT_FILENO, "#cisfun$ ", 9);
}

void execute_command(char **tokens) {
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        
        execve(tokens[0], tokens, NULL);

        
        perror("execve");
        _exit(EXIT_FAILURE);
    } else {
    
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
            write(STDOUT_FILENO, "./shell: No such file or directory\n", 36);
        }
    }
}

void get_tokens(char *input, char **tokens) {
    const char delimiter[] = " \t\n"; 
    size_t index = 0;

    tokens[index] = strtok(input, delimiter);
    while (tokens[index] != NULL && index < MAX_TOKENS - 1) {
        index++;
        tokens[index] = strtok(NULL, delimiter);
    }
    tokens[index] = NULL; 
}

int is_builtin_command(char *command) {
    return strcmp(command, "exit") == 0 || strcmp(command, "env") == 0;
}

void execute_builtin_command(char **tokens) {
    if (strcmp(tokens[0], "exit") == 0) {
        write(STDOUT_FILENO, "exit\n", 5);
        exit(EXIT_SUCCESS);
    } else if (strcmp(tokens[0], "env") == 0) {
        char **env = environ;
        while (*env != NULL) {
            write(STDOUT_FILENO, *env, strlen(*env));
            write(STDOUT_FILENO, "\n", 1);
            env++;
        }
    }
}

void execute_path_command(char **tokens) {
    char *path = getenv("PATH");
    char *path_copy = strdup(path);

    if (path_copy == NULL) {
        perror("strdup");
        exit(EXIT_FAILURE);
    }

    char *path_token = strtok(path_copy, ":");
    while (path_token != NULL) {
        char *full_path = malloc(strlen(path_token) + strlen(tokens[0]) + 2);
        if (full_path == NULL) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }

        sprintf(full_path, "%s/%s", path_token, tokens[0]);

        execve(full_path, tokens, NULL);
        free(full_path);
        path_token = strtok(NULL, ":");
    }
    write(STDOUT_FILENO, "./shell: No such file or directory\n", 36);

    free(path_copy);
}

int main() {
    char *input = NULL;
    size_t size_of_input = 0;
    char *tokens[MAX_TOKENS];

    while (1) {
        display_prompt();

        ssize_t read_result = getline(&input, &size_of_input, stdin);

        if (read_result == -1) {
            if (input == NULL) {
                write(STDOUT_FILENO, "exit\n", 5);
                break;
            } else {
                perror("getline");
                free(input);
                exit(EXIT_FAILURE);
            }
        }

        size_t length = strcspn(input, "\n");
        if (length > 0 && input[length - 1] == '\n') {
            input[length - 1] = '\0';
        }

        get_tokens(input, tokens);

        if (tokens[0] != NULL) {
            if (is_builtin_command(tokens[0])) {
                execute_builtin_command(tokens);
            } else {
                execute_path_command(tokens);
            }
        }
    }

    free(input);
    return EXIT_SUCCESS;
}