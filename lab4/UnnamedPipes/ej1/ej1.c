#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>

#define STD_IN   0
#define STD_OUT  1

int main(int argc, char **argv) {

    int fd_pipe[2];
    pid_t pid;

    if(argc < 2) {
        printf("Usage: %s <command1> <argument1> <command2> <argument2>\n", argv[0]);
        return -1;
    }

    if(pipe(fd_pipe) == -1) {
        perror("Can't open pipe");
        return -1;
    }

    pid = fork();

    switch(pid) {

        case -1:
            perror("Can't fork");
            exit(-1);

        case 0:
            /* Child */
            close(fd_pipe[1]);
            dup2(fd_pipe[0], STD_IN);
            close(fd_pipe[0]);


            if(execvp(argv[3], argv+3) == -1) {
                perror("Can't exec that");
                return -1;
            }

            break;

        default:
            /* Parent */
            close(fd_pipe[0]);
            dup2(fd_pipe[1], STD_OUT);
            close(fd_pipe[1]);
            
            if(execlp(argv[1], argv[1], argv[2], NULL) == -1) {
                perror("Can't exec that");
                return -1;
            }

            break;

    }

}


