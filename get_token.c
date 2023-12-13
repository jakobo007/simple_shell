#include "main.h"
/**
 * gettoken - Tokenizes the input and executes the appropriate action.
 * @input: The input string.
 */
void gettoken(char *input) {
const char delimiter[] = " \t\n";
char *token = strtok(input, delimiter);
char *args[MAX_INPUT_SIZE];
int i = 0;
while (token != NULL)
{
if (i >= MAX_INPUT_SIZE - 1)
{
fprintf(stderr, "Too many arguments\n");
}
args[i++] = token;
token = strtok(NULL, delimiter);
}
args[i] = NULL;
if (i > 0)
{
if (strcmp(args[0], "cd") == 0)
{
if (args[1] != NULL)
{
if (chdir(args[1]) != 0)
{
perror("cd");
}
}
else
{
fprintf(stderr, "cd: missing argument\n");
}
}
else if (strcmp(args[0], "exit") == 0)
{
exit(EXIT_SUCCESS);
}
else if (strcmp(args[0], "env") == 0)
{
environment();
}
else
{
execute_command(args[0], args);
}
}
}
