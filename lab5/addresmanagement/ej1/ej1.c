#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

#define NI_MAXHOST  1025
#define NI_MAXSERV    32


int main(int argc, char **argv) {

    if(argc < 2) {
        printf("Usage: %s <host>\n", argv[0]);
        return -1;
    }

    /*
           struct addrinfo {
               int              ai_flags;
               int              ai_family;
               int              ai_socktype;
               int              ai_protocol;
               socklen_t        ai_addrlen;
               struct sockaddr *ai_addr;
               char            *ai_canonname;
               struct addrinfo *ai_next;
           };

    */

    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));

    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = 0;
    hints.ai_protocol = 0;
    hints.ai_addr = NULL;
    hints.ai_canonname = NULL;
    hints.ai_next = NULL;

    struct addrinfo *result, *rp;
    int res = getaddrinfo(argv[1], NULL, &hints, &result);

    if(res != 0) {
        printf("getaddrinfo error: %s\n", gai_strerror(res));
        return -1;
    }

    for(rp = result; rp != NULL; rp = rp->ai_next) {
        
        if(rp->ai_family == AF_INET) {
            struct sockaddr_in *addr = rp->ai_addr;
            socklen_t addrlen = rp->ai_addrlen;
            char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];

            if(getnameinfo(addr, addrlen, hbuf, sizeof(hbuf), sbuf, sizeof(sbuf), NI_NUMERICHOST)) {
                perror("Can't resolve hostname");
                return -1;
            } else {
                printf("IPV4\t\thost: %s, serv=%s\t\t", hbuf, sbuf);
            }
        } else if(rp->ai_family == AF_INET6) {
            struct sockaddr_in6 *addr = rp->ai_addr;
            socklen_t addrlen = rp->ai_addrlen;
            char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];

            if(getnameinfo(addr, addrlen, hbuf, sizeof(hbuf), sbuf, sizeof(sbuf), NI_NUMERICHOST)) {
                perror("Can't resolve hostname");
                return -1;
            } else 
                printf("IPV6\t\thost: %s, serv=%s\t\t", hbuf, sbuf);
        }

        switch(rp->ai_socktype) {
            case SOCK_STREAM:
                printf("SOCK_STREAM\n");
                break;
            case SOCK_DGRAM:
                printf("SOCK_DGRAM\n");
                break;
            case SOCK_RAW:
                printf("SOCK_RAW\n");
                break;
            default:
                printf("Can't recognize socktype\n");
        }

    }

    freeaddrinfo(result);

    return 0;

}

