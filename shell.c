#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>


void handle_signal(int signol)
{
    if (signol == SIGINT || signol == SIGQUIT)
    {
        fprintf(stderr, "\n");
        exit(EXIT_SUCCESS);
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


        /** CHECK IF CMD EXIST **/
        if (access(input, X_OK) == -1) {
            fprintf(stderr, "%s: No such file or directory\n", "./shell");
            continue; 
        }




}
free(input);
return (0);
}
