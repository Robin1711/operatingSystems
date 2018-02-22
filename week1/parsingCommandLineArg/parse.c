#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "parse.h"
#include <string.h>

#define FALSE 0
#define TRUE 1
#define LSH_TOK_BUFSIZE 64

// Reads the line of the commandline and returns a string
char* read_line() {
  int bufferSize = 4;
  char* buffer = malloc(bufferSize* sizeof(char));
  assert(buffer != NULL);
  int c, position = 0;

  while (TRUE) {
    c = getchar();
    if (c == '"') continue;
    if (c == '\n' || c == EOF) {
      if (position > bufferSize-1) {
        bufferSize++;
        realloc(buffer, bufferSize* sizeof(char));
      }
      buffer[position] = '\0';
      break;
    } else {
      if (position > bufferSize-1) {
        bufferSize++;
        realloc(buffer, bufferSize* sizeof(char));
      }
      buffer[position] = c;
    }
    position++;
  }
  return buffer;
}

// Splits the line on spaces, and puts the multiple strings into an array of strings.
char** split_line(char *line) {
  int bufsize = LSH_TOK_BUFSIZE, position = 0;
  assert(bufsize != NULL);
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token;

  token = strtok(line, " ");
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += LSH_TOK_BUFSIZE;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      assert(tokens != NULL);
    }

    token = strtok(NULL, " ");
  }
  tokens[position] = NULL;
  return tokens;
}
