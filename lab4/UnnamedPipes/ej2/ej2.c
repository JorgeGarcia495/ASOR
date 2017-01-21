#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define MAX_MESSAGES     10
#define BUFFER_LENGTH   256
#define STD_IN   0

int main(int argc, char **argv) {

    int pc_pipe[2], cp_pipe[2]; /* [0] for reading, [1] for writing */
    pid_t pid;
    char buf[BUFFER_LENGTH];
    char msg;
    int i=0;
    ssize_t size;

    if(pipe(pc_pipe) == -1 || pipe(cp_pipe) == -1) {
        perror("Can't open pipe(s)");
        return -1;
    }

    pid = fork();

    switch(pid) {
        case -1:
            perror("Can't fork");
            return -1;
    
        case 0:
            /* Child */

            close(pc_pipe[1]);
            close(cp_pipe[0]);

            for(; i<MAX_MESSAGES; i++) {
                               
                size = read(pc_pipe[0], buf, BUFFER_LENGTH); /* The child will read the message from pipe */

                if(size == -1) {
                    perror("Child error. Can't read from pipe");
                    return -1;
                }

                buf[size] = '\0';

                printf("Child recieved: %s", buf); /* will write it to the standard output (terminal) */
                sleep(1); /* will wait for 1 second */

                if(i == MAX_MESSAGES-1)
                    msg = 'q';
                else
                    msg = 'r';
                
                if(write(cp_pipe[1], &msg, 1) == -1) { /* and will send the character to the parent process */
                    perror("Child error. Can't write to pipe");
                    return -1;
                }

            }

            close(pc_pipe[0]);
            close(cp_pipe[1]);
            return 0;
            break;

        default:
            /* Parent */
			msg = '\0';

            close(pc_pipe[0]);
            close(cp_pipe[1]);

            while(msg != 'q') {
                    
                printf("Enter message:\n");
            
                size = read(STD_IN, buf, BUFFER_LENGTH); /* The parent will read from the standard input */

                if(size == -1) {
                    perror("Parent error. Can't read from STDIN");
                    return -1;
                }

                buf[size] = '\0';

                if(write(pc_pipe[1], buf, size+1) == -1) { /* will send the message to child by writing to pipe */
                    perror("Parent error. Can't write to pipe");
                    return -1;
                }
                
	            if(read(cp_pipe[0], &msg, 1) == -1) { /* Then it will block waiting for the child's confirmation */
	                perror("Parent error. Can't read from pipe");
    	            return -1;
        	    }

				printf("'%c' recieved: %s\n", msg, (msg == 'r') ? "Continuing" : "Finalizing");
            }

            close(pc_pipe[1]);
            close(cp_pipe[0]);
            return 0;
            break;

    }

    return 0;
}
