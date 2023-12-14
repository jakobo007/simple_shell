#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#define MAX_INPUT_SIZE 1024





extern char **environ;

void handle_signal(int signol)
{
    if (signol == SIGINT || signol == SIGQUIT)
    {
        fprintf(stderr, "\n");
        exit(EXIT_SUCCESS);
    }
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
        wait(NULL);
    }
    }

}


void get_token(char *input){
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
        if (access(args[0], X_OK) == -1) {
            fprintf(stderr, "%s: No such file or directory\n", args[0]);

        } else {
               execute_command(args[0], args);
        }
    }
}

int main() {
char *input = NULL;
size_t size_of_input = 0;
size_t length;

signal(SIGINT, handle_signal);
signal(SIGQUIT, handle_signal);

while (1) {

 write(STDOUT_FILENO, "$ ", 2);
        if (getline(&input, &size_of_input, stdin) == -1) {
                if (feof(stdin)) {
                        break;
                } else {
                        perror("getline");
                        return (EXIT_FAILURE);
                }
        }

        length = strcspn(input, "\n");
        if (length > 0 && input[length - 1] == '\n') {
        input[length] = '\0';
        }



        get_token(input);

/**
        pid_t pid = fork();

        if (pid == -1) {
            perror("fork");
            return EXIT_FAILURE;
        } else if (pid == 0) {
            char *args[] = {input, NULL};
            execve(input, args, environ);
            perror("execve");
            _exit(EXIT_FAILURE);
        } else {
                wait(NULL);
        } **/
}
free(input);
return (0);
}