///Preparations
//headers
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>

//client data stract
typedef struct s_client {
    int id;
    char msg[1024];
} t_client;

//global variables
t_client clients[1024];
fd_set readSet, writeSet, activeSet;
int MaxFd, NextId = 0;
char BufRead[120000], BufWrite[120000];

//error printing
void ftError(char *s) {
    if (s)
        write(2, s, strlen(s));
    else
        write(2, "Fatal error", strlen("Fatal error"));
    write(2, "\n", 1);
    exit(1);
}

//sand data to all sockets
void sendAll(int n) {
    for (int i = 0; i < MaxFd; ++i)
        if (FD_ISSET(i, &writeSet) && i != n)
            send(i, BufWrite, strlen(BufWrite), 0);
}

///main loop
int main(int argc, char *argv[]) {
    //validation
    if (argc != 2)
        ftError("Wrong number of arguments");

    //create server socket
    int servFd = socket(2, 1, 0);
    if (servFd < 0)
        ftError(NULL);

    //prepare data to work
    FD_ZERO(&activeSet);
    bzero(&clients, sizeof(clients));
    MaxFd = servFd;
    FD_SET(servFd, &activeSet);
    //prepare server socket address
    struct sockaddr_in serverAdr;
    socklen_t len;
    bzero(&serverAdr, sizeof(serverAdr));
    serverAdr.sin_family = 2;
    serverAdr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    serverAdr.sin_port = htons(atoi(argv[1]));

    //bind and listen server socket
    if (bind(servFd, (const struct sockaddr *) &serverAdr, sizeof(serverAdr)) < 0)
        ftError(NULL);
    if (listen(servFd, 10) < 0)
        ftError(NULL);

    ///while loop
    while (1) {
        //update read and write fd by active
        readSet = writeSet = activeSet;

        //do select
        if (select(MaxFd + 1, &readSet, &writeSet, NULL, NULL) < 0)
            continue;
        for (int clientFd = 0; clientFd <= MaxFd; ++clientFd) {
            //check that socket can read and it is server socket
            if (FD_ISSET(clientFd, &readSet) && clientFd == servFd) {
                //accept socket and check on error
                int curFd = accept(servFd, ( struct sockaddr *)&serverAdr, &len);

                //update max fd
                MaxFd = MaxFd > curFd ? MaxFd : curFd;

                //set new client id
                clients[clientFd].id = NextId++;

                //add socket to active
                FD_SET(curFd, &activeSet);

                //create greetings
                sprintf(BufWrite, "server: client %d just arrived\n", clients[curFd].id);
                //send greetings
                sendAll(curFd);
                break;
            }
            //check that socket can read and it is also client socket
            if (FD_ISSET(clientFd, &readSet) && clientFd != servFd) {
                //do recv from client socket to bufferread
                int res = recv(clientFd, BufRead, 65536,0);
                //if recv failed -> error and close connection
                if (res <= 0) {
                    //create message
                    sprintf(BufWrite, "server: client %d just left\n", clientFd);
                    //send to all
                    sendAll(clientFd);
                    //clear and close socket
                    FD_CLR(clientFd, &activeSet);
                    close(clientFd);
                    break;
                } else {
                    //sand data to all clients
                    //
                    for (int i = 0, j = strlen(clients[clientFd].msg); i< res; ++i, ++j) {
                        //put data in client message
                        clients[clientFd].msg[j] = BufRead[j];
                        //check that end of message from buffer
                        if(clients[clientFd].msg[j] == '\n')
                        {
                            clients[clientFd].msg[j] = '\0';
                            //print data to all sockets
                            sprintf(BufWrite, "client %d: %s\n", clients[clientFd].id, clients[clientFd].msg);
                            sendAll(clientFd);
                            bzero(&clients[clientFd].msg, strlen(clients[clientFd].msg));
                            j =-1;
                        }
                    }
                }
                break;
            }
        }
    }
    return 0;
}