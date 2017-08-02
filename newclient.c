#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>

void error(char *msg){
	perror(msg);
	exit(1);
}

int main(int argc, char *argv[]){
	int sock;
	struct sockaddr_in server;
	struct hostent *hp;
	char buffer[256];
	int port = 6000;
	int msglen;

	bzero(buffer, 256);
	bzero((char *) &server, sizeof(server));	

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0)
		error("Socket Failed: ");

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;

	hp = gethostbyname("127.0.0.1");
	if(hp == NULL){
		perror("Get Host Failed\n");
		close(sock);
		exit(1);
	}
	memcpy(&server.sin_addr, hp->h_addr, hp->h_length);
	server.sin_port = htons(port);

	if(connect(sock, (struct sockaddr *) &server, sizeof(server)) < 0){
		perror("Connection Failed\n");
		close(sock);
		exit(1);
	}

	if(fork() > 0){
		while(1){
			bzero(buffer, 256);
			strcpy(buffer, "This is client.\n");
			send(sock, buffer, 255, 0);
			sleep(1);
		}
	}	
	while(1){
		msglen = recv(sock, buffer, 255, 0);
		if(msglen < 0){
			perror("Recieved Failed: ");
			close(sock);
			exit(1);
		}

		printf("Buffer: %s", buffer);
	}

/* 		bzero(buffer, 256); */
/* 		if((msglen = recv(sock, buffer, 256, 0)) < 0){ */
/* 			perror("Receive Failed: "); */
/* 			close(sock); */
/* 			exit(1); */
/* 		} */
/* 		msglen = 5; */
/* 		printf("Received from server: %s\t%d", buffer, msglen); */
/* 		printf("msglen: %d\n", msglen); */

		sleep(1);

	close(sock);
	return 0;
}
