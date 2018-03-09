#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

#include "parse.h"
#include "execute.h"
#include "help.h"

#define TRUE 1
#define FALSE 0

void sigHandler(int sig) {
  int status;
  if (sig == SIGCHLD) {
    do {
      wait(&status);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  } else {
    printf("Unfamiliar signal\n");
    exit(sig);
  }
}

int main(int argc, char **argv) {

  signal(SIGCHLD, sigHandler);
  char *line;
  char **args;
  int status = 1;

  do {
    printf("$ ");
    // Read the line and split it into arguments, which will be given to the function execute
    int numArgs;
    line = read_line();
    args = split_line(line, " ", &numArgs);

    if (strcmp(args[0], "exit") == 0) {
      printf("leaving commandline\n");
      status = 0;
    } else {
      status = execute(args, numArgs);
    }

    free(line);
    free(args);
  } while (status);

  return 0;
}