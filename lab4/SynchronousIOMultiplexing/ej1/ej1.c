#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
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
        return -1;
    }

    int pipe2 = open("pipe2", O_RDONLY | O_NONBLOCK);

    if(pipe2 == -1) {
        perror("Can't open FIFO");
        return -1;
    }



