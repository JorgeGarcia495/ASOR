#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>

int main(int argc, char **argv) {

    sigset_t blk_set;

    if(sigemptyset(&blk_set) == -1) {
        perror("Can't initialize set to empty");
        return -1;
    }

    if(sigaddset(&blk_set,SIGINT) == -1) {
        perror("Can't add SIGINT set");
        return -1;
    }

    if(sigaddset(&blk_set,SIGTSTP) == -1) {
        perror("Can't get SIGTSTP set");
        return -1;
    }

    if(sigprocmask(SIG_BLOCK, &blk_set, NULL) == -1) {
        perror("Can't block the signals");
        return -1;
    }

    char *sleep_secs = getenv("SLEEP_SECS");

    if(sleep_secs == NULL) {
        printf("Can't get SLEEP_SECS\n");
        printf("May be you need to specify env var:\n");
        printf("SLEEP_SECS=<value> %s\n", argv[0]);
        return -1;
    }

    int secs = atoi(sleep_secs);

    if(secs<0)
        secs=0;

    printf("I am going to sleep %i seconds now\n", secs);

    sleep(secs);

    sigset_t unblk_set;

    if(sigemptyset(&unblk_set) == -1) {
        perror("Can't initialize set to empty");
        return -1;
    }

    if(sigpending(&unblk_set) == -1) {
        perror("Can't check pending signals");
        return -1;
    }

    int is_sigint = sigismember(&unblk_set,SIGINT);
    int is_sigtstp = sigismember(&unblk_set,SIGTSTP);;

    if(is_sigint == -1) {
        perror("Can'r check SIGINT signal");
        return -1;
    } else if(is_sigint == 1)
        printf("SIGINT received while sleeping!\n");

    if(is_sigtstp == -1) {
        perror("Can't check SIGTSTP signal");
        return -1;
    } else if(is_sigtstp == 1) {
        printf("SIGTSTP received while sleeping!\n");
        sigdelset(&blk_set, SIGINT);

        if(sigprocmask(SIG_UNBLOCK, &blk_set, NULL) == -1) {
            perror("Can't unblock the signal");
            return -1;
        }
    }

    printf("And that's all folks!\n");

    return 0;
}
