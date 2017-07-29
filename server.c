#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main(int argc, char *argv[]){
	/* Variables */ 
	int sock;
	struct sockaddr_in server;
	int mysock;
	char buff[1024];
	int rval;

	/* Create Socket */
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0){
		perror("Failed to create socket");
		exit(1);
	}

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(5000);

	/* Call */ 
	if(bind(sock, (struct sockaddr *)&server, sizeof(server))){
		perror("Bind Failed");	
		exit(1);
	}

	/* Listen to network */ 
	listen(sock, 5);	
	
	/* Accept */ 	
	do{
		mysock = accept(sock, (struct sockaddr *)0, 0);
		if(mysock == -1){
			perror("Accept faield");
		}else{
			memset(buff, 0, sizeof(buff));
			if((rval = recv(mysock, buff, sizeof(buff), 0)) < 0){
				perror("Reading Stream Message Error");
			}else if(rval = 0){
				printf("Ending Connection\n");
			}else{
				printf("MSG: %s\n", buff);
			}
		
		
			printf("Got the message (rval = %d)\n", rval);
			close(mysock);
		}
	}while(1);

	return EXIT_SUCCESS;
}
