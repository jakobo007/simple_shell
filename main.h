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
void environment(void);
void execute_CMD_PATH(char *cmd, char *args[]);
void execute_command(char *command, char *args[]);
void gettoken(char *input);

#endif
