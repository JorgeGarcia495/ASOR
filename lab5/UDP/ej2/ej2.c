#include <time.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include <arpa/inet.h>
#include <sys/types.h>
#include <netdb.h>

#define BUFFER_LENGTH  256

int main(int argc, char **argv) {

    int sd;
    ssize_t bytes;
    char buffer[BUFFER_LENGTH];
    struct addrinfo hints, *rc, *result;
    char host[NI_MAXHOST], serv[NI_MAXSERV];
    int finish = 0;
    time_t stime;
    struct sockaddr_storage client;
    socklen_t client_len = sizeof(client);

    if(argc != 4) {
        printf("Usage: %s <address> <port> <command>\n", argv[0]);
        return -1;
    }

    hints.ai_flags    = AI_PASSIVE; // Return 0.0.0.0 or ::
    hints.ai_family   = AF_UNSPEC;  // IPv4 or IPv6
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = 0;

    rc = getaddrinfo(argv[1], argv[2], &hints, &result);

    if(rc != 0) {
        printf("Error getting addrinfo: %s\n", gai_strerror((int)rc));
        return -1;
    }

    sd = socket(result->ai_family, result->ai_socktype, 0);

    if(sd == -1) {
        perror("Can't open the socket");
        return -1;
    }

    if(sendto(sd, argv[3], strlen(argv[3]), 0, result->ai_addr, result->ai_addrlen) == -1) {
        perror("Error sending to server");
        return -1;
    }

    bytes = recvfrom(sd, buffer, 80, 0, result->ai_addr, &(result->ai_addrlen));
    buffer[bytes] = '\0'; 

    if(bytes < 0) {
        perror("Couldn't recieve");
        close(sd);
        return -1;
    }

    printf("Server sent: \t%s\n", buffer);
    freeaddrinfo(result);
    close(sd);
    return 0;

}
