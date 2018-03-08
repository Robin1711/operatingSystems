#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/mman.h>
#include <signal.h>
#include <sys/types.h>

void child() { 
    kill(getpid(), SIGSEGV);
    exit(EXIT_SUCCESS);
}

void sigsegv_handler(int signum) { 
    printf("got a sigsegv\n");
    kill(getppid(), SIGUSR1);
    return;
}

void sigusr1_handler(int signum) { 
    printf("got a sigusr1\n");
    return;
}

int main(int argc, char *argv[]) {
    sigset_t mask;
    sigfillset(&mask);
    signal(SIGUSR1, sigusr1_handler);

    sigdelset(&mask, SIGUSR1);

    if (fork() == 0) { 
        child();
    }

    sigsuspend(&mask);

    printf("after sigusr1\n");
}