#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

#include "parse.h"
#include "execute.h"

#define TRUE 1
#define FALSE 0

void sigHandler(int sig) {
//  printf("\nsigHandler called\n");
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
  char* fname = malloc (100 * sizeof(char));
  for (int i = 0; i < length; i++) {
    fname[0] = '\0';
    strcat(fname, searchPaths[i]);
    strcat(fname, "/");
    strcat(fname, file);
    if( access(fname, F_OK) != -1 ) {
      return fname;
    }
  }
  free(fname);
  return NULL;
}

int main(int argc, char **argv) {

  //  signal(SIGCHLD, sigHandler);
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
      // args[0] is the program itsself
      char* x = filePath(args[0]);
      if (x != NULL) {
        status = execute(x, args);
//        status = executeRedirect(args[0], args[1], args[2]);
      }
    }
    free(line);
    free(args);
  } while (status);

  return 0;
}