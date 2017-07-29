#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

#define DATA "Hello There"

int main(int agrc, char *argv[]){
	int sock;
	struct sockaddr_in server;
	struct hostent *hp;
	char buff[1024];

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0){
		perror("Socket Failed");
		exit(1);
	}

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = 5000;
	
	hp = gethostbyname(argv[1]);
	if(hp == 0){
		perror("gethostbyname failed");
		close(sock);
		exit(1);
	}
	memcpy(&server.sin_addr, hp->h_addr, hp->h_length);
	server.sin_port = htons(5000);

	if(connect(sock, (struct sockaddr *) &server, sizeof(server)) < 0){
		perror("Connection Failed");
		close(sock);
		exit(1);
	}

	int g = send(sock, argv[2], sizeof(*argv[2]) * strlen(argv[2]), 0);
	if(g < 0){
		perror("Send Failed");
		close(sock);
		exit(1);
	}

	printf("Sent: %s\n", argv[2]);
	close(sock);
	return EXIT_SUCCESS;
}
