#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parse.h"
#include <unistd.h>

// Execute executes the command in arguments[0] with the flags arguments[1..n]
int execute(char** arguments) {
  pid_t pid, wpid;
  int status;
  char path[1024]; //Path to file
  if (getcwd(path, sizeof(path)) == NULL) perror("getcwd() error");
  strcat(path, "/");
  strcat(path, arguments[0]);

  printf("%s\n", path);
  strcat(path,"/");
  strcat(path, arguments[0]);
  if (getcwd(path, sizeof(path)) == NULL) perror("getcwd() error");
  pid = fork();
  if (pid == 0) {
    // Child process
    if (execv(path, arguments) == -1) {
      printf("execute: command %s not found\n", arguments[0]);
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    // Error forking
    perror("lsh");
  } else {
    // Parent process
    do {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}
int main(int argc, char **argv) {
  char path[1024];
  if (getcwd(path, sizeof(path)) == NULL) perror("getcwd() error");
  strcat(path, "/");

  char *line;
  char **args;
  int status = 1;

  // Jump into virtual shell
  do {
    printf("$ ");
    // Read the line and split it into arguments, which will be given to the function execute
    line = read_line();
    args = split_line(line);

    if (strcmp(args[0], "exit") == 0) {
      printf("leaving commandline\n");
      status = 0;
    } else {
      status = execute(args);
    }
    free(line);
    free(args);
  } while (status);

  return 0;
}
