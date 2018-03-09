#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <assert.h>

#include "execute.h"
#include "parse.h"
#include "help.h"

#define TRUE 1
#define FALSE 0


// filePath returns the path to a specified file
// If the file does not exist, the function returns NULL
char* filePath(char* file) {
  // Gather all paths possible for the file to be in
  char **searchPaths;
  int length;
  char* paths = getenv("PATH");
  char extraPath[1024];
  if (getcwd(extraPath, sizeof(extraPath)) == NULL) perror("getcwd() error");

  strcat(paths, ":");
  strcat(paths, extraPath);

  searchPaths = split_line(paths, ":", &length);

  // Check for all paths which path contains the file
  char* fname = malloc(100 * sizeof(char));
  for (int i = 0; i < length; i++) {
    fname[0] = '\0';
    strcat(fname, searchPaths[i]);
    strcat(fname, "/");
    strcat(fname, file);
//    printf("%s\n", fname);
    if( access(fname, F_OK) != -1 ) {
      return fname;
    }
  }
  free(fname);
  return NULL;
}

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

void setInput(char* fromfile) {
  close(STDIN_FILENO);

  int fd = open(fromfile, O_RDONLY);
  if (fd == -1) {
    printf("%s", strerror(errno));
    exit(fd);
  }
}

// from the arguments given, return an array with the arguments from idx start up to and including end
char** getArguments(char** arguments, int start, int length) {
  char **resultingArgs = malloc(length+1 * sizeof(char*));
  resultingArgs[length] = NULL;
  for(int a = 0; a < length; a++) {
    resultingArgs[a] = strdup(arguments[a+start]);
    assert(resultingArgs[a] != NULL);
  }
  return resultingArgs;
}

int checkArgumentAtPosition(char** arguments, char* compareArg, int position) {
  //  return whether the last argument is '&'
  char* argAtPos = arguments[position];
  if (strcmp(argAtPos, compareArg) == 0) {
    return TRUE;
  }
  return FALSE;
}

int isBackGroundProcess(char** arguments, int numArgs) {
  //  return whether the last argument is '&'
  char* lastArg = arguments[numArgs-1];
  char* enpercent = malloc(sizeof(char));
  enpercent[0] = '&';
  if (strcmp(lastArg, enpercent) == 0) {
    return TRUE;
  }
  return FALSE;
}