#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>
#include <string.h>

void myhandler(int signal) {
    if(signal == SIGUSR1) {
        printf("Auto-destruction stoped!\n");
        exit(0);
    }
}

int main(int argc, char **argv) {

    if(argc != 2) {
        printf("Usage: %s <secs>\n", argv[0]);
        return -1;
    }

    int secs = atoi(argv[1]);

    struct sigaction handler;

    handler.sa_sigaction = &myhandler;
    handler.sa_flags = SA_SIGINFO;


    sigset_t mask;
    sigemptyset(&mask);

    if(sigprocmask(SIG_UNBLOCK, &mask, NULL) == -1) {
        perror("Can't unblock file");
        return -1;
    }
    
    if(sigaction(SIGUSR1, &handler, NULL) == -1) {
        perror("Can't set SIGUSR1 handler");
        return -1;
    }

    sleep(secs);

    if(unlink(argv[0]) == -1) {
        perror("Can't delete file");
        return -1;
    }

    printf("Self deleted\n");


    return 0;
}
