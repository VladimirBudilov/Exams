#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>

typedef struct s_client{
	int id;
	char msg[1024];
} t_client;

t_client clients[1024];
char toread[120000], towrite[120000];
fd_set readset, writeset, activeset;
int max = 0, next = 0;

void fterror(char *s){
	s ? write(2, s, strlen(s)) : write(2, "Fatal error", strlen("Fatal error"));
	write(2, "\n", 1);
	exit(1);
}

void sendall(int n){
	for(int i = 0; i <= max; i++)
		if(FD_ISSET(i, &writeset) && i != n)
			send(i, towrite, strlen(towrite), 0);
}
int main(int argc, char **argv){
	if(argc !=2)
		fterror(0);
	int serv = socket(2,1,0);
	if(serv < 0)
		fterror(0);
	FD_ZERO(&activeset);
	bzero(&clients, sizeof(clients));
	max = serv;
	FD_SET(serv, &activeset);
	struct sockaddr_in adr;
	socklen_t len;
	bzero(&adr, sizeof(adr));
	adr.sin_family = 2;
	adr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	adr.sin_port = htons(atoi(argv[1]));
	if(bind(serv, (const struct sockaddr *)&adr, sizeof(adr)) < 0)
		fterror(0);
	if(listen(serv, 10) < 0)
		fterror(0);
	while(1){
		readset = writeset = activeset;
		if(select(max + 1, &readset, &writeset,0 ,0) < 0)
			continue;
		for(int fd = 0; fd <= max; fd++){
			if(FD_ISSET(fd, &readset) && fd == serv){
				int cur = accept(fd, (struct sockaddr *)&adr, &len);
				if(cur < 0)
					continue;
				max = max > cur ? max : cur;
				clients[cur].id = next++;
				FD_SET(cur, &activeset);
				sprintf(towrite, "server: client %d just arrived\n", clients[cur].id);
				sendall(cur);
				break;
			}
			if(FD_ISSET(fd, &readset) && fd != serv){
				int res = recv(fd, toread, 65536, 0);
				if(res <= 0){
					sprintf(towrite, "server: client %d just left\n", clients[fd].id);
					sendall(fd);
					FD_CLR(fd, &activeset);
					close(fd);
					break;
				}
				else{
					for(int i = 0, j = strlen(clients[fd].msg); i < res; i++,j++){
						clients[fd].msg[j] = toread[i];
						if(clients[fd].msg[j] == '\n'){
							clients[fd].msg[j] = '\0';
							sprintf(towrite, "client %d: %s\n", clients[fd].id, clients[fd].msg);
							sendall(fd);
							bzero(clients[fd].msg, strlen(clients[fd].msg));
							j =-1;
						}}break;}}}}}
