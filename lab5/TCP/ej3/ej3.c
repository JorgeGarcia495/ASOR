#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>

#include <string.h>
#include <errno.h>
#include <netdb.h>

#define BUFFER_LENGTH    256

int main(int argc, char **argv) {

    int sd, client_sd;
    ssize_t bytes;
    char buffer[BUFFER_LENGTH];
    struct addrinfo hints, *rc, *result;
    char host[NI_MAXHOST], serv[NI_MAXSERV];
//    int finish = 0;
    time_t stime;
    fd_set set;

    struct sigaction handler;

    memset(&handler, '\0', sizeof(handler));

    handler.sa_handler = SIG_DFL;
    handler.sa_flags=SA_NOCLDWAIT;

    if(sigaction(SIGCHLD, &handler, NULL) == -1) {
        perror("Can't avoid zombie apocalypse");
        exit(EXIT_FAILURE);
    }

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

    listen(sd, 5);
    
    struct sockaddr_storage client;
    socklen_t client_len = sizeof(client);
    pid_t pid;
    int status;

    while (1) {
      client_sd = accept(sd, (struct sockaddr *) &client, &client_len);
      getnameinfo((struct sockaddr *) &client, client_len, host, NI_MAXHOST, serv, NI_MAXSERV, NI_NUMERICHOST|NI_NUMERICSERV);

      printf("Connection from %s:%s\n", host, serv);

      pid = fork();
      switch(pid) {
              
              case -1:
                  perror("Can't fork");
                  exit(EXIT_FAILURE);
                  break;
    
              case 0:
                  /* Child: handle connection */
    
                  while(1) {
                      bytes = recv(client_sd, buffer, BUFFER_LENGTH, 0); // Byte stream!
                      buffer[bytes] = '\0';
                      if(bytes < 0) {
                          printf("Error recieving from %s:%s\t%s\n", host, serv, strerror(errno));
                          close(sd);
                          exit(EXIT_FAILURE);
                      }
                      if(bytes!=0) {
                          if(send(client_sd, buffer, bytes, 0) == -1) {
                              printf("Error sending to %s:%s\t%s\n", host, serv, strerror(errno));
                              close(sd);
                              exit(EXIT_FAILURE);
                          }
                      }else {
                          printf("Connection from %s:%s terminated\n", host, serv);
                          exit(EXIT_SUCCESS);
                      }
                  }
      }
    }

    close(sd);
    return 0;
}
