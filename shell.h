
#ifndef SHELL_C
#define SHELL_C

#define MAX_PATH_LEN 1000

void parse_args( char * line, char ** arg_ary, char * s );
void printPath();
void exitShell();
void cd(char directory[], int shouldPrint);
void redirectR(char* cmd, char* output, char** args);
void redirectL(char* cmd, char* input, char** args);
void pipeP(char* cmd1, char* cmd2);
int err();
#endif
