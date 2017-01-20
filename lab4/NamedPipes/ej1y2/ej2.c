#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, char **argv) {

    if(argc != 2) {
        printf("Usage: %s <string>\n", argv[0]);
        return 0;
    }

    int fd = open("pipe", O_WRONLY);

    if(fd == -1) {
        perror("Can't open file");
        close(fd);
        return -1;
    }

    if(write(fd,argv[1],strlen(argv[1])) == -1) {
            perror("Can't write on file");
            close(fd);
            return -1;
    }

    close(fd);
    
    return 0;

}

