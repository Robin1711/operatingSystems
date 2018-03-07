#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parse.h"
#include <unistd.h>

// Execute executes the command in arguments[0] with the flags arguments[1..n]
int execute(char** arguments) {
  pid_t pid, wpid;
  int status;

  pid = fork();
  if (pid == 0) {
    // Child process
    if (execvp(arguments[0], arguments) == -1) {
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
  char *line;
  char **args;
  int status = 1;

  // Jump into virtual minimal shell
  do {
    printf("$ ");
    // Read the line and split it into arguments, which will be given to the function execute
    line = read_line();
    args = split_line(line);
    if (strcmp(args[0], "execute") == 0) {
      status = execute(args+1);
    } else if (strcmp(args[0], "exit") == 0) {
      printf("leaving commandline\n");
      status = 0;
    } else {
      printf("incorrect command\n");
    }
    free(line);
    free(args);
  } while (status);

  return 0;
}
