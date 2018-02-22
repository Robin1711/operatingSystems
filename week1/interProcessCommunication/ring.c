#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define TRUE 1
#define FALSE 0
#define WRITE 1
#define READ 0

// As long as necesary, the child reads, prints and writes the sum.
void action(int totalChildren, int child, int p[][2]) {
  int sum = 0;
  int neighbour = (child + 1) % totalChildren;
  while (sum - 1 + totalChildren <= 50) {
    // read from pipe
    int bytesread = read(p[child][READ], &sum, sizeof(int));
    if (bytesread != sizeof(int)) printf("Bytes read != sizeof(int) -> %d != %lu", bytesread, sizeof(int));

    // print and writ to pipe from neighbour
    printf("pid=%d: %d\n", getpid(), sum);
    sum++;
    int bytesWritten = write(p[neighbour][WRITE], &sum, sizeof(int));
    if (bytesWritten != sizeof(int)) printf("Bytes written != sizeof(int) -> %d != %lu", bytesWritten, sizeof(int));
  }
}

void performRing(int n) {
  pid_t pid[n], wpid;
  int status;
  int p[n][2];
  pid_t parentpid = getpid();

  // Parent initializes the pipe
  for (int i = 0; i < n; i++) {
    if (pipe(p[i]) < 0) {
      perror("pipe error.");
      exit(1);
    }
  }
  // Parent writes initial value for first child
  int sum = 0;
  int bytesWritten = write(p[0][WRITE], &sum, sizeof(int));
  if (bytesWritten != sizeof(int)) printf("Bytes written != sizeof(int) -> %d != %lu", bytesWritten, sizeof(int));

  // Fork children
  for (int i = 0; i < n; i++) {
    pid[i] = fork();
    if (pid[i] == 0) {
      action(n, i, p);
      exit(1);
    } else if (pid < 0) {
      perror("lsh");
    }
  }

  // Parent waits for all children to finish and closes the pipes
  for (int i = 0; i < n; i++) {
    wpid = waitpid(pid[i], &status, WUNTRACED);
  }
  for (int i = 0; i < n; i++) {
    close(p[i][READ]);
    close(p[i][WRITE]);
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
