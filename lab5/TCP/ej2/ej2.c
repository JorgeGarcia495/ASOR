#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <stdio.h>

#include <arpa/inet.h>
#include <string.h>
#include <errno.h>

#define BUFFER_LENGTH    256
#define STD_IN             0


int main(int argc, char **argv) {

    int sd, client_sd;
    ssize_t bytes;
    char buffer[BUFFER_LENGTH];
    struct addrinfo hints, *rc, *result;
    char host[NI_MAXHOST], serv[NI_MAXSERV];
    int finish = 0;
    time_t stime;
    fd_set set;

    if(argc != 3) {
        printf("Usage %s <address> <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    
    hints.ai_flags    = AI_PASSIVE; // Return 0.0.0.0 or ::
    hints.ai_family   = AF_UNSPEC;  // IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM;
    
    rc = getaddrinfo(argv[1], argv[2], &hints, &result);

    if(rc != 0) {
        printf("Error getting addrinfo: %s\n", gai_strerror((int)rc));
        exit(EXIT_FAILURE);
    }

    sd = socket(result->ai_family, result->ai_socktype, 0);
    
    if(sd == -1) {
        perror("Can't open the socket");
        exit(EXIT_FAILURE);
    }

    if(connect(sd, (struct sockaddr *) result->ai_addr, result->ai_addrlen)) {
        perror("Can't connect");
        close(sd);
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(result);

    while(!finish) {
        int size = read(STD_IN,&buffer,BUFFER_LENGTH);
        
        if(size == -1) {
            perror("Can't read from stdin\n");
            close(sd);
            exit(EXIT_FAILURE);
        }

        buffer[size] = '\0';
//        printf("We have read: %s, sized in %i\n", buffer, size);

        if(buffer[0] == 'Q' && buffer[1]== '\n' && size == 2) {
            close(sd);
            finish++;
        } else {

//            printf("We are about to send %s, sized in %i\n", buffer, size);

            if(send(sd, buffer, (size+1), 0) == -1) {
                perror("Error sending");
                close(sd);
                exit(EXIT_FAILURE);
            }
//            printf("We have sent %s, sized in %i\n", buffer, (size+1));
    
            size = recv(sd, buffer, BUFFER_LENGTH, 0);
    
            if(size == -1)  {
                perror("Can't read from socket");
                close(sd);
                exit(EXIT_FAILURE);
            }
    
            buffer[size] = '\0';
            printf("%s", buffer);
        }
    }

    close(sd);
    return 0;
}
