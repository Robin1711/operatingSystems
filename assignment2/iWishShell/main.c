#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parse.h"
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

// With background process, put ouput in > /dev/null
// a.out < in > out

int executeRedirect(char *command, char *fromfile, char *tofile) {
  pid_t pid, wpid;
  int status;

  pid = fork();
  if (pid == 0) {
    // Child process
    close(STDIN_FILENO);
    int fd = open(fromfile, O_RDONLY);
    if (fd == -1) {
      printf("%s", strerror(errno));
      exit(fd);
    }

    close(STDOUT_FILENO);
    fd = open(tofile, O_WRONLY | O_CREAT | O_TRUNC, 644);
    if (fd == -1) {
      printf("%s", strerror(errno));
      exit(fd);
    }

//    close(STDOUT_FILENO);
//    dup(output_pipe[1]);

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
int execute(char **arguments) {
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
    // Parent process
    do {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}

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

int main(int argc, char **argv) {
  char path[1024];
  if (getcwd(path, sizeof(path)) == NULL) perror("getcwd() error");
  strcat(path, "/");

//  signal(SIGCHLD, sigHandler);
  char *line;
  char **args;
  int status = 1;

  // Jump into virtual shell
  do {
    printf("$ ");
    // Read the line and split it into arguments, which will be given to the function execute
    line = read_line();
    args = split_line(line);

    if (strcmp(args[0], "exit") == 0) {
      printf("leaving commandline\n");
      status = 0;
    } else {
//      status = execute(args);
      status = executeRedirect(args[0], args[1], args[2]);
    }
    free(line);
    free(args);
  } while (status);

  return 0;
}
