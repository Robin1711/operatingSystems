#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define TRUE 1
#define FALSE 0

void doChildAction(int totalChildren, int child, int* p) {
  int sum = 0;
  while(TRUE) {
    // read from pipe
    int bytesread = read(p[0], &sum, sizeof(int));
    if (bytesread != sizeof(int)) printf("Bytes read != sizeof(int) -> %d != %d", bytesread, sizeof(int));

    // decide whether to continue or not
    if (sum > 50) {
      int bytesWritten = write (p[1], &sum, sizeof(int));
      if (bytesWritten != sizeof(int)) printf("Bytes written != sizeof(int) -> %d != %d", bytesWritten, sizeof(int));
      return;
    }

    // check whether it is our turn to print and increment
    if(sum % totalChildren == child) {
      printf("pid=%d: %d\n", getpid(), sum);
      sum++;
    }

    // write the result back
    int bytesWritten = write (p[1], &sum, sizeof(int));
    if (bytesWritten != sizeof(int)) printf("Bytes written != sizeof(int) -> %d != %d", bytesWritten, sizeof(int));
  }
}

void performRing(int n) {
  pid_t pid[n], wpid;
  int status;
  int p[2];
  pid_t parentpid = getpid();

  // parent initializes the pipe
  if(pipe(p) < 0) {
    perror("pipe error.");
    exit(1);
  }
  int sum = 0;
  int bytesWritten = write (p[1], &sum, sizeof(int));
  if (bytesWritten != sizeof(int)) printf("Bytes written != sizeof(int) -> %d != %d", bytesWritten, sizeof(int));

  // fork children
  for (int i=0; i<n; i++) {
    pid[i] = fork();
    if (pid[i] == 0) {
      doChildAction(n, i, p);
      exit(1);
    } else if (pid < 0) {
      perror("lsh");
    }
  }

  if(getpid() == parentpid) {
    for (int i=0; i<n; i++) {
      wpid = waitpid(pid[i], &status, WUNTRACED);
    }
  }
}

int main(int argc, char **argv) {
  int processes;
  scanf("%d", &processes);
  if (processes < 2 || processes > 16) {
    printf("processes not between 2 and 16: %d\n", processes);
    exit(-1);
  }
  performRing(processes);
  return 0;
}
