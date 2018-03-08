#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include "execute.h"

#define TRUE 1
#define FALSE 0

void setOutput(char* tofile) {
  close(STDOUT_FILENO);

  int mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
  int oflags = O_WRONLY | O_CREAT | O_TRUNC;

//    fd = open(tofile, O_WRONLY | O_CREAT | O_TRUNC, 0644);

  int fd = open(tofile, oflags, mode);
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

int executeBackground(char **arguments) {
  pid_t pid, wpid;
  int status;

  pid = fork();
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
    // Parent process does nothing
    // When the process is done, a signal handler for the signal SIGCHLD is called
//    do {
//      wpid = waitpid(pid, &status, WUNTRACED);
//    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
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
int execute(char* filePath, char **arguments) {
  pid_t pid, wpid;
  int status;
  char path[1024];
  printf("executing: %s\n", filePath);
  pid = fork();
  if (pid == 0) {
    // Child process
    if (execv(filePath, arguments) == -1) {
      printf("PROBLEM: command %s is not executed\n", arguments[0]);
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