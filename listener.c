#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

#define DATA "Hello There"
#define true 1
#define BUFFER_SIZE 2048

int main(int agrc, char *argv[]){
	int sock;
	struct sockaddr_in server;
	struct hostent *hp;
	char buff[1024];

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0){
		perror("Socket Failed\n");
		exit(1);
	}

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = 5000;
	
	hp = gethostbyname(argv[1]);
	if(hp == 0){
		perror("gethostbyname failed\n");
		close(sock);
		exit(1);
	}
	memcpy(&server.sin_addr, hp->h_addr, hp->h_length);
	server.sin_port = htons(5000);

	if(connect(sock, (struct sockaddr *) &server, sizeof(server)) < 0){
		perror("Connection Failed\n");
		close(sock);
		exit(1);
	}

	int read_size;
	char server_message[BUFFER_SIZE];

	memset(server_message, 0, BUFFER_SIZE);

	while(true)
	{
		if(recv(sock , server_message , BUFFER_SIZE , 0) > 0 )
		{
			fflush(stdout);
			printf("> %s\n", server_message);
		}
	}

	close(sock);
	return EXIT_SUCCESS;
}
