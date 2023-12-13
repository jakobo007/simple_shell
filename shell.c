#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
/**
 * main - The entry point of the simple shell program.
 * Responsible for reading user input, tokenizing, and executing commands.
 * Returns: Always returns 0.
 */
int main() {
char *input = NULL;
size_t size_of_input = 0;
signal(SIGTERM, end_of_file);
signal(SIGINT, new_line);
while(1)
{
write(STDOUT_FILENO, "$ ", 2);
fflush(stdout);
if (getline(&input, &size_of_input, stdin) == -1)
{
perror("getline");
exit(EXIT_FAILURE);
}
input[strcspn(input, "\n")] = '\0';
gettoken(input);
}
free(input);
return (0);
}