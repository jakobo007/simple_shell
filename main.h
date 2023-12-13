#ifndef _MAIN_H_
#define _MAIN_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

extern char **environ;
#define MAX_INPUT_SIZE 1024

void new_line();
void end_of_file();
void print_environment();
void execute_cmd_path(char *cmd, char *args[]);
void execute_command(char *command, char *args[]);
void tokenize_and_execute(char *input);

#endif
