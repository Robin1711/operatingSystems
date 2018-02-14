#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <unistd.h>

int length(char* array) {
  int i = 0, length = 0;
  while (array[i] != NULL) {
    length++;
    i++;
  }
  return length;
}

char* copyArray (char* original) {
  int size = length(original);
  char* copy = calloc(size, sizeof(char));
  for (int i = 0; i < size; i++) {
    copy[i] = original[i];
  }
  assert(copy != NULL);
  return copy;
}

int execute(char* prog, char* command) {
  pid_t pid, wpid;
  int status;

  pid = fork();
  // Now two processes: current and child
  if (pid == 0) {
    // Child process

//    if (execl(getenv("PATH"), "-c", "ls", (const char *)0)) {
//      perror("lsh");
//    }
//    if (execve(getenv("PATH"), command, 0)) {
//      perror("lsh");
//    }
    char** arr = calloc(1, sizeof(char*));
    assert(arr != NULL);
    arr[0] = command;

    if (execvp(prog, arr)) {
      perror("lsh");
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

int main(int argc, char **argv) {
//  Read

  printf("%d\n", argc);
  char* command = copyArray(argv[1]);

  // pass the program and the command to execute
  execute(argv[0], command);
  free(command);

//  Parse
//  Execute
  return 0;

}
