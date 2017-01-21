#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUFFER_LENGTH   256

int main() {

    /* We better create here the FIFOs */

    if(mkfifo("pipe",0644) == -1) {
        perror("Can't create FIFO");
        return -1;
    }

    if(mkfifo("pipe2",0644) == -1) {
        perror("Can't create FIFO");
        return -1;
    }

    char buffer[BUFFER_LENGTH];

    int pipe = open("pipe", O_RDONLY | O_NONBLOCK);

    if(pipe == -1) {
        perror("Can't open FIFO");
        close(pipe);
        return -1;
    }

    int pipe2 = open("pipe2", O_RDONLY | O_NONBLOCK);

    if(pipe2 == -1) {
        perror("Can't open FIFO");
        close(pipe);
        close(pipe2);
        return -1;
    }
    
    int sel, current;

    do {

        fd_set pipes;

        FD_ZERO(&pipes);
        FD_SET(pipe, &pipes);
        FD_SET(pipe2, &pipes);

        int max, nump;

        if(pipe < pipe2) 
            max = pipe2 + 1;
        else
            max = pipe + 1;

        sel = select(max, &pipes, NULL, NULL, NULL);

        if(sel > 0) {

            if(FD_ISSET(pipe, &pipes))
                current = pipe;
            else if(FD_ISSET(pipe2, &pipes))
                current = pipe2;

            ssize_t toread = BUFFER_LENGTH;
            
            while(toread == BUFFER_LENGTH) {

                toread = read(current, buffer, BUFFER_LENGTH);

                if(toread == -1) {
                    perror("Can't read from FIFO");
                    close(pipe);
                    close(pipe2);
                    return -1;
                }

                buffer[toread] = '\0';
                printf("Pipe number %i contains: %s\n", (current==pipe) ? 1 : 2, buffer);
            }

            if(current == pipe) {
                close(pipe);
                if((pipe = open("pipe", O_RDONLY | O_NONBLOCK)) == -1) {
                        perror("Can't open pipe");
                        close(pipe);
                        close(pipe2);
                        return -1;
                }
            } else if(current == pipe2) {
                close(pipe2);
                if((pipe2 = open("pipe2", O_RDONLY | O_NONBLOCK)) == -1) {
                        perror("Can't open pipe");
                        close(pipe);
                        close(pipe2);
                        return -1;
                }
            }
                

        }
    } while(sel != -1);

    perror("Error in selection");

    close(pipe);
    close(pipe2);
    return -1;
}
