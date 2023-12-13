#ifndef _MAIN_H_
#define _MAIN_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#define MAX_INPUT_SIZE 1024
extern char **environ;

void new_line();
void gettoken(char *input);
void execute_command(char *command, char *args[]);
void execute_CMD_PATH(char *cmd, char *args[]);
void environment();
void end_of_file();

#endif