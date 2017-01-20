#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <string.h>

volatile int sigints=0;
volatile int sigtstps=0;

void myhandler(int signal) {
    switch(signal) {
        case SIGINT:
            sigints++;
            break;
        case SIGTSTP:
            sigtstps++;
            break;
        default:
            break;
    }
}
int main() {

    struct sigaction handler;

    memset(&handler, '\0', sizeof(handler));

    handler.sa_sigaction=&myhandler;
    handler.sa_flags=SA_SIGINFO;

    if(sigaction(SIGINT, &handler, NULL) == -1) {
        perror("Can't set SIGINT handler");
        return -1;
    }

    if(sigaction(SIGTSTP, &handler, NULL) == -1) {
        perror("Can't set SIGTSTP handler");
        return -1;
    }

    sigset_t sigset;

    if(sigemptyset(&sigset) == -1) {
        perror("Can't set empty set");
        return -1;
    }

    while(sigints+sigtstps < 10)
        sigsuspend(&sigset);

    printf("We have captured %i SIGINT signals,\n and %i SIGTSTP signals\n", sigints, sigtstps);

    return 0;
}
