#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include "execute.h"
#include "parse.h"
#include "help.h"

#define TRUE 1
#define FALSE 0

void childAction(char** arguments, int numArgs) {
//  int length_command = 0;
//  for (int a = 0; a < numArgs; a++) {
//    if(checkArgumentAtPosition(arguments, ">", a)){
//      a++;
//      setOutput(arguments[a]);
//    } else if (checkArgumentAtPosition(arguments, "<", a)){
//      a++;
//      setInput(arguments[a]);
//    } else {
//      length_command++;
//    }
//  }

  char** command = getArguments(arguments, 0, numArgs);
  for (int i = 0; i < numArgs; i++) {
    printf("%s\n", command[i]);
    printf("%s\n", arguments[i]);
  }

//  char** command = arguments;
//  for (int i = 0; i < numArgs; i++) {
//    printf("%s, ", command[i]);
//    printf("%s\n", arguments[i]);
//  }


  char* pathToFile = filePath(command[0]);
  printf("path == %s\n", pathToFile);
  if (execv(pathToFile, command) == -1) {
    printf("PROBLEM: command %s is not executed\n", command[0]);
  }
  //free(command);
}

int executeRedirect(char *command, char *inputFile, char *outputFile) {
  pid_t pid, wpid;
  int status;

  pid = fork();
  if (pid == 0) {
    // Child process
    setInput(inputFile);
    setOutput(outputFile);
    if (execvp(command, NULL) == -1) {
      printf("execute: command %s not found\n", command);
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

// Execute executes the command in arguments[0] with the flags arguments[1..n]
int execute(char **arguments, int numArgs) {
  int runInBackGround = checkArgumentAtPosition(arguments, "&", numArgs-1);

  pid_t pid, wpid;
  int status;

  pid = fork();
  if (pid == 0) {
    // Child process
    if (runInBackGround) {
      char** commandStuff;
      char backGroundOutputFile[10];
      backGroundOutputFile[0] = '\0';
      strcat(backGroundOutputFile, "/dev/null");
//      setOutput(backGroundOutputFile);
      commandStuff = getArguments(arguments, 0, numArgs-1);
      childAction(commandStuff, numArgs-1);
      free(commandStuff);
    } else {
      childAction(arguments, numArgs);
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    // Error forking
    perror("lsh");
  } else {
    // Parent process
    if (!runInBackGround) {
      do {
        wpid = waitpid(pid, &status, WUNTRACED);
      } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
  }

  return 1;
}
