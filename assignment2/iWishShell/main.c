#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "parse.h"
#include "helperFuntions.h"

#define TRUE 1
#define FALSE 0

// The following function is a handler fro the signal SIGCHLD
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

// The function runInBackground takes the arguments of a to-be-executed program and makes sure the output
// is discarded and the '&' is removed from the command.
void runInBackGround(char** arguments, int numArgs) {
  // Set output to /dev/null
  char backGroundOutputFile[10];
  backGroundOutputFile[0] = '\0';
  strcat(backGroundOutputFile, "/dev/null");
  setOutput(backGroundOutputFile);

  // remove & from argumentlist
  char **childProcessArguments = getArguments(arguments, 0, numArgs - 1);
  execute(childProcessArguments, numArgs - 1);
  free(childProcessArguments);
}

// The function distribute takes the arguments of a to-be-executed program and makes sure about whether it
// is ran in the back ground and forks the children necessary for the processes to run
// NOTE: This is only one, since the piping is not implemented
int distribute(char **arguments, int numArgs) {
  int isBackGroundProcess = checkArgumentAtPosition(arguments, "&", numArgs - 1);

  pid_t pid, wpid;
  int status;

  pid = fork();
  if (pid == 0) {
    // Child process
    if (isBackGroundProcess) {
      runInBackGround(arguments, numArgs);
    } else {
      execute(arguments, numArgs);
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    // Error forking
    perror("lsh");
  } else {
    // Parent process
    if (!isBackGroundProcess) {
      do {
        wpid = waitpid(pid, &status, WUNTRACED);
      } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
  }

  return 1;
}

int main(int argc, char **argv) {
  char *line;
  char **args;
  int status = 1;

  printf("\nStarting the shell, use \"exit\" to exit the shell.\n\n");

  do {
    printf("$ ");
    // Read the line and split it into arguments, which will be given to the function execute
    int numArgs;
    line = read_line();
    if (strcmp(line, "") != 0) {
      args = split_line(line, " ", &numArgs);

      // if the command is not to exit the shell, distribute the command(s) over process(es)
      if (strcmp(args[0], "exit") == 0) {
        printf("leaving commandline\n");
        status = 0;
      } else {
        distribute(args, numArgs);
      }

      for (int i = 0; i < numArgs; i++) free(args[i]);
      free(args);
    }
  } while (status);

  return 0;
}