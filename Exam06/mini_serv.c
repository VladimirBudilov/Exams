#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>


///Data structure for clients
typedef struct s_clients {
    int     id;
    char    msg[1024];
} t_clients;

///Create 1024 clients
t_clients   clients[1024];

///fd_set is Data structure for select and handles file descriptors
fd_set      readfds, writefds, active;
///fdMax is max file descriptor and idNext is next id for new client
int         fdMax = 0, idNext = 0;
///bufferRead is buffer for read and bufferWrite is buffer for write
char        bufferRead[120000], bufferWrite[120000];

///Function for error handling
void    ftError(char *str) {
    if (str)
        write(2, str, strlen(str));
    else
        write(2, "Fatal error", strlen("Fatal error"));
    write(2, "\n", 1);
    exit(1);
}

///Function for sending message to all clients except one
void    sendAll(int not) {
    for(int i = 0; i <= fdMax; i++)
        if(FD_ISSET(i, &writefds) && i != not)
            send(i, bufferWrite, strlen(bufferWrite), 0);
}

int main(int ac, char **av) {
    ///Check number of arguments
    if (ac != 2)
        ftError("Wrong number of arguments");

    ///Create listening server socket
    int servfd = socket(AF_INET, SOCK_STREAM, 0);
    if (servfd < 0)
        ftError(NULL);

    ///clear active fd_set
    FD_ZERO(&active);
    ///clear clients array
    bzero(&clients, sizeof(clients));
    ///update max number of file descriptor
    fdMax = servfd;
    ///add server socket to active fd_set
    FD_SET(servfd, &active);

    ///Create server address
    struct sockaddr_in  servaddr;
    socklen_t           len;
    ///clear server address
    bzero(&servaddr, sizeof(servaddr));
    ///set server address family, ip and port
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); //127.0.0.1
    servaddr.sin_port = htons(atoi(av[1]));

    ///Bind and listen server socket. If error - exit
    if ((bind(servfd, (const struct sockaddr *)&servaddr, sizeof(servaddr))) < 0)
        ftError(NULL);
    if (listen(servfd, 10) < 0)
        ftError(NULL);

    ///Main loop
    while(1) {
        ///Copy active fd_set to read and write fd_set
        readfds = writefds = active;
        ///Select active file descriptors and wait for events on them (read/write)
        if (select(fdMax + 1, &readfds, &writefds, NULL, NULL) < 0)
            continue;
        ///Check all file descriptors for events and handle them
        for(int clientFd = 0; clientFd <= fdMax; clientFd++) {
            ///Check if client has data to read and read it
            if (FD_ISSET(clientFd, &readfds) && clientFd == servfd) {
                ///Accept new client
                int connfd = accept(servfd, (struct sockaddr *)&servaddr, &len);
                ///Check for error
                if (connfd < 0)
                    continue;
                ///Update max number of file descriptor and set new client id
                fdMax = connfd > fdMax ? connfd : fdMax;
                ///Set new client id
                clients[connfd].id = idNext++;
                ///Add new client to active fd_set
                FD_SET(connfd, &active);
                sprintf(bufferWrite, "server: client %d just arrived\n", clients[connfd].id);
                sendAll(connfd);
                break;
            }
            ///Check if client has data to read and read it
            if (FD_ISSET(clientFd, &readfds) && clientFd != servfd) {
                ///Read data from client
                int res = recv(clientFd, bufferRead, 65536, 0);
                if (res <= 0) {
                    sprintf(bufferWrite, "server: client %d just left\n", clients[clientFd].id);
                    sendAll(clientFd);
                    FD_CLR(clientFd, &active);
                    close(clientFd);
                    break;
                }
                else {
                    ///Add new data to client message and check if it is full message (with \n)
                    for (int i = 0, j = strlen(clients[clientFd].msg); i < res; i++, j++) {
                        clients[clientFd].msg[j] = bufferRead[i];
                        if (clients[clientFd].msg[j] == '\n') {
                            clients[clientFd].msg[j] = '\0';
                            sprintf(bufferWrite, "client %d: %s\n", clients[clientFd].id, clients[clientFd].msg);
                            sendAll(clientFd);
                            bzero(&clients[clientFd].msg, strlen(clients[clientFd].msg));
                            j = -1;
                        }
                    }
                    break;
                }
            }
        }
    }
}