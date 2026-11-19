#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include "shell.h"

int err(){ // errno function whenever something goes wrong, usually forking
    printf("errno %d\n", errno);
    printf("%s\n", strerror(errno));
    exit(1);
}

void parse_args( char * line, char ** arg_ary, char * s){ // parse the arguments from stdin for our shell; line is the input, arg_ary is the argument array of strigs, and s is the delimiter
  char *str;
  int i = 0;
  while((str = strsep(&line, s))!=NULL){
    arg_ary[i] = str;
    i++;
  }
  arg_ary[i] = NULL;
}


void printPath(){ // prints the cwd
  char cwd[MAX_PATH_LEN];
  if (getcwd(cwd, sizeof(cwd)) != NULL){
    char *pathp = cwd;
    if (strstr(cwd, getenv("HOME"))){
      pathp += strlen(getenv("HOME"));
    }
    printf("%s", pathp);
  } else {
      perror("getcwd() err");
  }
}

void cd(char directory[], int shouldPrint){ // directory: the argument after the cd is ran, shouldPrint: accounts for -n flag. Our cd implementation to change directories, there is no * wildcard implementation
  char path[MAX_PATH_LEN];


  if(directory == NULL ){
    if(chdir(getenv("HOME"))!= 0){
      perror("change directory fail");
    }
    if(shouldPrint){
      printPath();
    }
    
    return;
  }

  else  if (*directory == '~'){
    char* homePath = getenv("HOME");


    strcpy(path, homePath);

    if(*(directory+1)  != '\0'){
      strcat(path, directory + 1);
    }
    if(chdir(path) != 0){
      perror("change directory fail");

    }
    if(shouldPrint){
      printPath();
    }
    return;
  }

  else{
    if(chdir(directory) !=0){
      perror(("change directory fail"));

    }
    // printPath();
    return;
  }

}

void exitShell(){ //exits for ctrl-d and exit command
  exit(0);
}

void redirectR(char* cmd, char* output, char** args){ // cmd: the command running, output: the file overwritten with stdout, args: arguments of the command. This is our implementation for the > redirect
  pid_t p = fork();
  if (p < 0){
    err();
  }
  else if (p == 0){
    int fd = open(output, O_WRONLY | O_TRUNC | O_CREAT, 0644);
    if(fd < 0) err();
    dup2(fd, STDOUT_FILENO);
    close(fd);
    execvp(cmd, args);
    err();
  }
  else{
    waitpid(p, NULL, 0);
  }
}

void redirectL(char* cmd, char* input, char** args){ // cmd: command with the redirect, input: replacement for stdin, args: arguments for command. This is our redirection for the < redirect
  pid_t p = fork();
  if (p < 0){
    err();
  }
  else if (p == 0){
    int fd = open(input, O_RDONLY);
    if(fd < 0) err();
    dup2(fd, STDIN_FILENO);
    close(fd);
    execvp(cmd, args);
    err();
  }
  else{
    waitpid(p, NULL, 0);
  }
}

void pipeP(char* cmd1, char* cmd2){// cmd1: first command, cmd2: second command that receives. This is our piping implementation
  char buffer[1000];
  FILE *f1 = popen(cmd1, "r");
  if (f1 == NULL) err();
  FILE *f2 = popen(cmd2, "w");
  if (f2 == NULL) err();

  while (fgets(buffer, sizeof(buffer), f1)){
    fputs(buffer, f2);
  }
  pclose(f1);
  pclose(f2);
}
