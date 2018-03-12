#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "parse.h"

#define FALSE 0
#define TRUE 1
#define INIT_BUFSIZE 4

// Reads the line of the commandline and returns a string
char* read_line() {
  int bufferSize = 4;
  char* buffer = malloc(bufferSize * sizeof(char));
  assert(buffer != NULL);
  int c, position = 0;

  while (TRUE) {
    c = getchar();
    if (c == '"') {
      continue;
    }
    if (c == '\n' || c == EOF) {
      if (position > bufferSize-1) {
        bufferSize++;
        buffer = realloc(buffer, bufferSize* sizeof(char));
      }
      buffer[position] = '\0';
      break;
    } else {
      if (position > bufferSize-1) {
        bufferSize *= 2;
        realloc(buffer, bufferSize* sizeof(char));
      }
      buffer[position] = c;
    }
    position++;
  }
  return buffer;
}

// Splits the line on spaces. Returns a completely new allocated 2D char
// array, in which the input is copied according to the splitToken.
char** split_line(char* line, char* splitToken, int* length) {
  int bufsize = INIT_BUFSIZE;
  int position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token;

  // Make sure the original line is not modified.
  char* copy_of_line = strdup(line);

  token = strtok(copy_of_line, splitToken);
  (*length) = 0;
  while (token != NULL) {
    (*length)++;
    tokens[position] = strdup(token);
    position++;

    if (position >= bufsize) {
      bufsize += INIT_BUFSIZE;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      assert(tokens != NULL);
    }

    token = strtok(NULL, splitToken);
  }
  tokens[position] = NULL;
  free(copy_of_line);
  return tokens;
}

// Check whether an argument at the specified position is the specified argument
int checkArgumentAtPosition(char** arguments, char* compareArg, int position) {
  char* argAtPos = arguments[position];
  if (strcmp(argAtPos, compareArg) == 0) {
    return TRUE;
  }
  return FALSE;
}

// From the arguments given, return a newely created array with the n arguments starting from idx start
char** getArguments(char**source, int start, int nArgs) {

  char **target = malloc((nArgs + 1) * sizeof(char*));
  assert(target != NULL);
  for(int a = 0; a < nArgs; a++) {
    target[a] = strdup(source[start + a]);
    strcat(target, "\0");
    assert(target[a] != NULL);
  }
  target[nArgs] = NULL;


  return target;
}

// Append an argument to a given source arguments list
void appendArgument(char** sourceArgs, int* args, char* newArg) {
  // + 1 for new argument, +1 for asuring null termination
  sourceArgs = realloc(sourceArgs, (*args + 2) * sizeof(char*));
  assert(sourceArgs != NULL);

  sourceArgs[*args] = strdup(newArg);
  sourceArgs[*args+1] = NULL;
  (*args)++;
  return;
}
