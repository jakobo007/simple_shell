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
            execve(command, args, environ);
            perror(args[0]);
            exit(EXIT_FAILURE);
        } else {
        wait(NULL);
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
    ssize_t read_result;
    size_t length;

    while (1) {
        signal(SIGINT, handle_signal);
        signal(SIGQUIT, handle_signal);

        write(STDOUT_FILENO, "$ ", 2);

        read_result = getline(&input, &size_of_input, stdin);

        if (read_result == -1) {
            if (input == NULL) {
                write(STDOUT_FILENO, "End of file\n", 12);
                break;
            } else {
                perror("getline");
                free(input);
                return EXIT_FAILURE;
            }
        }

        if (input != NULL) {
            length = strcspn(input, "\n");
            if (length > 0 && input[length - 1] == '\n') {
                input[length] = '\0';
                get_token(input);
            }
        }
    }

    free(input);
    return 0;
}


/***
int main() {
char *input = NULL;
size_t size_of_input = 0;
size_t length;



while (1) {

    signal(SIGINT, handle_signal);
signal(SIGQUIT, handle_signal);

 write(STDOUT_FILENO, "$ ", 2);
        if (getline(&input, &size_of_input, stdin) != -1) {
                if (input == NULL) {
                    write(STDOUT_FILENO, "End of file\n", 12);
                        break;
                } else if (input != NULL) {
                    length = strcspn(input, "\n");
                    if (length > 0 && input[length - 1] == '\n') {
                    input[length] = '\0';
                get_token(input);
                            }
                }
                else {
                        perror("getline");
                        free(input);
                        return (EXIT_FAILURE);
                }                
        }

}
free(input);
return (0);
}

**/