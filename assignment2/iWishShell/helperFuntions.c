#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include "parse.h"
#include "helperFuntions.h"

#define TRUE 1
#define FALSE 0

// execute executes the given command with its arguments and makes sure it takes
// the input from the correct file and writes the output to the correct file
void execute(char **arguments, int numArgs) {
  int commandLength = 0;
  for (int a = 0; a < numArgs; a++) {
    if(checkArgumentAtPosition(arguments, ">", a)){
      a++;
      setOutput(arguments[a]);
    } else if (checkArgumentAtPosition(arguments, "<", a)){
      a++;
      setInput(arguments[a]);
    } else {
      commandLength++;
    }
  }

  char** command = getArguments(arguments, 0, commandLength);
  char* pathToFile = filePath(command[0]);

  if (execv(pathToFile, command) == -1) {
    printf("PROBLEM: command %s is not executed\n", command[0]);
  }
}

// Sets the ouput to where this writes
void setOutput(char* tofile) {
  close(STDOUT_FILENO);

  int mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
  int oflags = O_WRONLY | O_CREAT | O_TRUNC;

  int fd = open(tofile, oflags, mode);
//  fd = open(tofile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (fd == -1) {
    printf("%s", strerror(errno));
    exit(fd);
  }
}

// Sets the input from where this reads
void setInput(char* fromfile) {
  close(STDIN_FILENO);

  int fd = open(fromfile, O_RDONLY);
  if (fd == -1) {
    printf("%s", strerror(errno));
    exit(fd);
  }
}

// filePath returns the path to a specified file
// If the file does not exist, the function returns NULL
char* filePath(char* file) {
  // Gather all paths possible for the file to be in
  char* paths = getenv("PATH");

  // Append the current working directory to the paths to search
  char curWD[1024];
  if (getcwd(curWD, sizeof(curWD)) == NULL) perror("getcwd() error");
  strcat(paths, ":");
  strcat(paths, curWD);

  int length;
  char** searchPaths = split_line(paths, ":", &length);

  // Check for all paths which path contains the file
  char* fname = malloc(100 * sizeof(char));
  for (int i = 0; i < length; i++) {
    fname[0] = '\0';
    strcat(fname, searchPaths[i]);
    strcat(fname, "/");
    strcat(fname, file);
    if( access(fname, F_OK) != -1 ) {
      strcat(fname, "\0");
      return fname;
    }
  }
  free(fname);
  return NULL;
}
