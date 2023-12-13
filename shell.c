#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>


extern char **environ;

#define MAX_INPUT_SIZE 1024

void display_prompt() {
    write(STDOUT_FILENO, "#cisfun$ ", 9);
}

int main(void) {
    char input[MAX_INPUT_SIZE];

    while (1) {
        pid_t pid;

        display_prompt();

        if (read(STDIN_FILENO, input, MAX_INPUT_SIZE) == 0) {

            write(STDOUT_FILENO, "\n", 1);
            break;
        }


        input[strcspn(input, "\n")] = '\0';

        pid = fork();

        if (pid == -1) {
            char *error_msg = "./shell: fork: error\n";
            write(STDERR_FILENO, error_msg, strlen(error_msg));
            exit(EXIT_FAILURE);
        }

        if (pid == 0) {
            if (execve(input, NULL, environ) == -1) {
                char *error_msg = "./shell: No such file or directory\n";
                write(STDERR_FILENO, error_msg, strlen(error_msg));
                exit(EXIT_FAILURE);
            }
        } else {
          int status;
            waitpid(pid, &status, 0);

            if (WIFEXITED(status) && WEXITSTATUS(status) == 127) {
                char *error_msg = "./shell: No such file or directory\n";
                write(STDERR_FILENO, error_msg, strlen(error_msg));
            }
        }
    }

    return EXIT_SUCCESS;
}
