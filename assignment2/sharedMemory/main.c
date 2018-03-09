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
pid_t pid_child0, pid_child1;
int shared_parent = 0;
int SIGNAL;
int[2] pingpong;


void procPingPong(int whoami, volatile int*sharedTurnVariable) {
	/*whoami == 0 or whoami == 1*/
	int msg = 0;
	while (msg < 5) {
		while (*sharedTurnVariable != whoami); /*busy-waiting*/
		printf (whoami == 0 ? "Ping\n" : "...Pong\n");
		*sharedTurnVariable = 1 - whoami;
		write(pingpong[1], (void*)sharedTurnVariable, sizeof(int));
		//send signal SIGRTMIN to parent to let parent know it should read from the pipe
		kill(getppid(), SIGRTMIN);
		mprotect((void*)sharedTurnVariable, pagesize, PROT_NONE);
		msg++;
	}
	exit(EXIT_SUCCESS);
}

void handler_child1(int sig, siginfo_t *si, void*unused) {
	//send signal to parent to ask for access memory
	if (sig == SIGSEGV) {
		int status_kill = kill(getppid(), SIGNAL);
	}
	//if access granted, set mprotect to write and read from pipe
	if (sig == SIGNAL) {
		//mprotect to write
		//we would like to update the rights of sharedTurnVariable to write instead of none, however we don't have its address
		//read from pipe
		//we would like to update sharedTurnVariable by reading the pipe, however we don't have its address
	}
}

void parent_handler(int sig, siginfo_t *si, void*unused){
	if (sig == SIGUSR1){
		//first child wants access to memory
		//if child has this right, write the memory to pipe and send signal to child
		if (shared_parent == 0)
		{
			//write to pipe
			write(pingpong[1], &shared_parent, sizeof(int));
			//send signal to child			
			int status_kill = kill(pid_child0, SIGUSR1);
			if(status_kill < 0){
				printf("status of kill: %d %s\n", status, strerror(errno));
				exit(-1);
			}
		}
	}
	if (sig == SIGUSR2){
		//second child wants access to memory
		//if child has this right, write the memory to pipe and send signal to child
		if (shared_parent == 1)
		{		
			write(pingpong[1], &shared_parent, sizeof(int));	
			int status_kill = kill(pid_child1, SIGUSR2);
			if (status_kill < 0){
				printf("status of kill: %d %s\n", status, strerror(errno));
				exit(-1);
			}
		}
	}
	if (sig == SIGRTMIN){
		//read from pipe and update value shared_parent
		read(pingpong[0], &shared_parent, sizeof(int));
	}
}


int main(int argc, char *argv[]) {
  void *buffer;
  int *p;
  struct sigaction sa;
  int status;
  
  sa.sa_flags = SA_SIGINFO;
  sigemptyset(&sa.sa_mask);
  sa.sa_sigaction = parent_handler;
  sigaction(SIGUSR1, &sa, NULL);
  sigaction(SIGUSR2, &sa, NULL);
  sigaction(SIGRTMIN, &sa, NULL);
  
  pagesize = sysconf(_SC_PAGE_SIZE);

  /* Allocate a buffer aligned on a page boundary;
   * default initial protection is PROT_READ | PROT_WRITE 
   */
  if (posix_memalign(&buffer, pagesize, 4*pagesize) != 0) {
    fprintf(stderr, "Fatal error: aligned memory allocation failed.\n");
    exit(EXIT_FAILURE);
  }
  
  p = buffer;
  mprotect(buffer, 4*pagesize, PROT_NONE);

  pipe(pingpong);
  
  if ((pid_child0 = fork()) == 0) {
	SIGNAL = SIGUSR1;
	sa.sa_sigaction = handler_child1;
	sigaction(SIGSEGV, &sa, NULL);
	sigaction(SIGNAL, &sa, NULL);
    procPingPong(0, p);
    exit(1);
  }
  if ((pid_child1 = fork()) == 0) {
	SIGNAL = SIGUSR2;
	sa.sa_sigaction = handler_child2;
	sigaction(SIGSEGV, &sa, NULL);
	sigaction(SIGNAL, &sa, NULL);
    procPingPong(1, p);
    exit(1);
  }

  waitpid(-1, &status, 0);
  waitpid(-1, &status, 0);
  return EXIT_SUCCESS;
  
  printf("Loop completed\n");     /* Should never happen */
  return 0;
}
