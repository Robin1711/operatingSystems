#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <string.h>

int pagesize;
int cnt = 0;
int pingpong[2];

void procPingPong(int whoami, volatile int*sharedTurnVariable) {
	/*whoami == 0 or whoami == 1*/
	int msg = 0;
	while (msg < 5) {
		while (*sharedTurnVariable != whoami); /*busy-waiting*/
		printf (whoami == 0 ? "Ping\n" : "...Pong\n");
		*sharedTurnVariable = 1 - whoami;
		write(pingpong[1], (void*)sharedTurnVariable, sizeof(int));
		mprotect((void*)sharedTurnVariable, pagesize, PROT_READ);
		msg++;
	}
	exit(EXIT_SUCCESS);
}

void handler(int sig, siginfo_t *si, void *unused) {
  int* value = si->si_addr;
  int status = read(pingpong[0], value, sizeof(int));
  printf("reading: %d %s\n", status, strerror(errno));
  printf("Got SIGSEGV at address: 0x%lx\n", (long) si->si_addr);
  mprotect(si->si_addr, pagesize, PROT_WRITE);
  cnt++;
  if (cnt == 10) {
    exit(EXIT_FAILURE);
  }
}


int main(int argc, char *argv[]) {
  void *buffer;
  int *p;
  struct sigaction sa;
  
  int status;
  
  sa.sa_flags = SA_SIGINFO;
  sigemptyset(&sa.sa_mask);
  sa.sa_sigaction = handler;
  sigaction(SIGSEGV, &sa, NULL);
  
  pagesize = sysconf(_SC_PAGE_SIZE);

  /* Allocate a buffer aligned on a page boundary;
   * default initial protection is PROT_READ | PROT_WRITE 
   */
  if (posix_memalign(&buffer, pagesize, 4*pagesize) != 0) {
    fprintf(stderr, "Fatal error: aligned memory allocation failed.\n");
    exit(EXIT_FAILURE);
  }

  printf("Start of region:        0x%lx\n", (long) buffer);
  mprotect(buffer, 4*pagesize, PROT_READ);
  
  p = buffer;
  printf("p: %d\n", *p);

  //pipe(chanPing);
  //pipe(chanPong);
  pipe(pingpong);
  int status_write = write(pingpong[1], (void*)p, sizeof(int));
  printf("writing: %d %s\n", status_write, strerror(errno));
  if (fork() == 0) {
    procPingPong(0, p);
  }
  if (fork() == 0) {
    procPingPong(1, p);
  }
  waitpid(-1, &status, 0);
  waitpid(-1, &status, 0);
  return EXIT_SUCCESS;
  
  printf("Loop completed\n");     /* Should never happen */
  return 0;
}
