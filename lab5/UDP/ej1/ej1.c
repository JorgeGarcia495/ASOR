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

    if(argc != 3) {
        printf("Usage: %s <address> <port>\n", argv[0]);
        return -1;
    }

    hints.ai_flags    = AI_PASSIVE; // Return 0.0.0.0 or ::
    hints.ai_family   = AF_UNSPEC;  // IPv4 or IPv6
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = 0;
    /*hints.ai_addr = NULL;
    hints.ai_canonname = NULL;
    hints.ai_next = NULL;*/

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

    if(bind(sd, (struct sockaddr *) result->ai_addr, result->ai_addrlen)) {
        perror("Can't bind");
        return -1;
    }

    freeaddrinfo(result);

    struct sockaddr_storage client;
    socklen_t client_len = sizeof(client);

    while (!finish) {
        bytes = recvfrom(sd, buffer, 80, 0, (struct sockaddr *) &client, &client_len);
        buffer[bytes] = '\0'; 

        if(bytes < 0) {
            perror("Couldn't recieve");
            close(sd);
            return -1;
        } else {
            if(getnameinfo((struct sockaddr *) &client, client_len, host, NI_MAXHOST, serv, NI_MAXSERV, NI_NUMERICHOST|NI_NUMERICSERV) != 0) {
                perror("Error getting nameinfo");
                return -1;
            }
            printf("\tMessage (%i bytes) from %s:%s: %s\n", bytes, host, serv, buffer);

            switch(buffer[0]) {

                case 't':
                    /* Time requested */
                    time(&stime);
                    char currenttime[25];
                    strftime(currenttime, 10, "%H:%M:%S", localtime(&stime));
                    if(sendto(sd, currenttime, strlen(currenttime)+1, 0, (struct sockaddr *) &client, client_len) == -1) {
                        perror("Error sending time");
                        return -1;
                    }
                    break;

                case 'd':
                    /* Date requested */
                    time(&stime);
                    char currentdate[25];
                    strftime(currentdate, 11, "%Y:%m:%d", localtime(&stime));
                    if(sendto(sd, currentdate, strlen(currentdate)+1, 0, (struct sockaddr *) &client, client_len) == -1) {
                        perror("Error sending date");
                        return -1;
                    }
                    break;

                case 'q':
                    /* Quit requested */
                    printf("Quiting...\n");
                    finish = 1;
                    break;

                default:
                    printf("Unknown command: %s\n", buffer);
                    break;

            }
        }
    }

    close(sd);
    return 0;

}
