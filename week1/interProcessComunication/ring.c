#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

void doRing(int processes) {
  pid_t pid, wpid;
  int status;
  pid_t pids[processes];

/* Start children. */
  for (int p = 0; p < processes; p++) {
    if ((pids[p] = fork()) < 0) {
      perror("fork");
      abort();
    }
  }

  pid = fork();
  if (pid == 0) {
    // Child process
    printf("process = %d", pid);

//    if (execvp(arguments[0], arguments) == -1) {
//      perror("lsh");
//    }
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
}

int main(int argc, char **argv) {
  int processes;
  scanf("%d", &processes);
  if (processes < 2 || processes > 16) {
    printf("processes not between 2 and 16: %d\n", processes);
    exit(-1);
  }
  doRing(processes);
  return 0;
}
