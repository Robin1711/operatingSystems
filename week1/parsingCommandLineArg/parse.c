#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "parse.h"
#include <string.h>

#define INIT_BUFFER_SIZE 4
#define FALSE 0
#define TRUE 1

void free2Darray(int **ar, int rows){
  int row;
  for(row=0; row<rows; row++){
    free(ar[row]);
  }
  free(ar);
}

char **allocate2Darray(int rows, int columns){
  char **new;
  int row;
  new = calloc(rows,sizeof(char*));
  assert(new != NULL);
  for(row=0; row<rows; row++){
    new[row] = calloc(columns,sizeof(char));
    assert(new[row] != NULL);
  }
  return new;
}

char *read_line_simple() {
  char *line = NULL;
  ssize_t bufsize = 0; // have getline allocate a buffer for us
  getline(&line, &bufsize, stdin);
  return line;
}

char* read_line() {
  int bufferSize = 4;
  char* buffer = malloc(bufferSize* sizeof(char));
  assert(buffer != NULL);
  int c, position = 0;

  while (TRUE) {
    c = getchar();
    if (c == '""') continue;
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

#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"
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
