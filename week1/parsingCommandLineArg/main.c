#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "parse.h"
#include <sys/types.h>
#include <unistd.h>

int execute(char** arguments) {
  pid_t pid, wpid;
  int status;

  pid = fork();
  // Now two processes: current and child
//  char** arr = calloc(1, sizeof(char*));
//  arr[0] = calloc(2, sizeof(char));
//  arr[0][0] = 'l';
//  arr[0][1] = 's';
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
//  Read
  char *line;
  char **args;
  int status = 1;

  do {
    printf("$ ");
    line = read_line();
    args = split_line(line);
//    status = execute(args);
    if (strcmp(args[0], "execute") == 0) {
      status = execute(args+1);
    } else {
      printf("incorrect command");
      status = 0;
    }
    free(line);
    free(args);
  } while (status);
  return 0;
}
