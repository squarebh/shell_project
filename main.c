#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "shell.h"

int main(int argc, char const *argv[]) { //main to run everything
  char line[256];
  int printPrompt = 1;

  if(argc > 1 && strcmp(argv[1], "-n") == 0){
    printPrompt = 0;
  }

  while(1){
    if(printPrompt){
      printPath();
    printf("$ ");
    }
    fflush(stdout);

    if(fgets(line, sizeof(line), stdin) == NULL) break;
    line[strcspn(line, "\r\n")] = '\0';

    char * cmds[100];
    parse_args(line, cmds, ";");

    for(int i = 0; cmds[i]; i++){
      char * args[100];
      parse_args(cmds[i], args, " ");
      if(args[0] == NULL) exitShell();

      if(strcmp(args[0], "cd") == 0){
        if (args[1] == NULL){
          printf("No such file or directory\n");
          continue;
        }
        cd(args[1], printPrompt);
        continue;
      }

      if(strcmp(args[0], "exit") == 0){
        exitShell();
        continue;
      }

      int ran = 0;
      int pipeInd = 0;
      int pipe = 0;
      char left[256] = "";
      char right[256] = "";
      for(int j = 0; args[j] != NULL; j++){
        if( strcmp(args[j], "|") == 0){
          pipeInd = j;
          pipe = 1;
          break;
        }
      }
      if(pipe == 1){
        if (args[0] == NULL || args[pipeInd+1] == NULL){
          printf("Syntax Error\n");
          continue;
        }
        for(int j = 0; j < pipeInd; j++){
          if(j>0){
            strcat(left," ");
          }
          strcat(left,args[j]);
        }
        for(int j = pipeInd + 1; args[j] != NULL; j++){
          if(j>pipeInd+1){
            strcat(right," ");
          }
          strcat(right,args[j]);
        }
        pipeP(left,right);
        continue;
      }
      for(int j = 0; args[j] != NULL && !ran; j++){
        if( strcmp(args[j], "<") == 0){
          if (args[0] == NULL || args[j+1] == NULL){
            printf("Syntax Error\n");
            ran = 1;
            break;
          }
          args[j] = NULL;
          redirectL(args[0], args[j+1], args);
          ran = 1;
          break;
        }
      }
      for(int j = 0; args[j] != NULL && !ran; j++){
        if( strcmp(args[j], ">") == 0){
          if (args[0] == NULL || args[j+1] == NULL){
            printf("Syntax Error\n");
            ran = 1;
            break;
          }
          args[j] = NULL;
          redirectR(args[0], args[j+1], args);
          ran = 1;
          break;
        }
      }

      if (ran) continue;

      pid_t p = fork();
      if (p < 0){
        err();
        exitShell();
      }
      else if (p == 0){
        execvp(args[0], args);
        perror("execvp failed");
        exit(1);
      }
      else{
        waitpid(p, NULL, 0);
      }
    }
  }
  return 0;
}
