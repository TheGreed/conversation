#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define MAX_CLIENTS 10

int *all_clients;
int n_clients;
key_t key;
int shmid;
int shm_size;
char *shm;

void send_handler(char *buffer){
	int i = 0;
	int *p = NULL;
	while((p = (all_clients + i * sizeof(int))) && *p > 0){
		send(*p, buffer, strlen(buffer), 0);
		++i;
	}	
}

void recv_handler(int asock){
	int msglen;
	char buffer[256];
	while(1){
		bzero(buffer, 256);
		msglen = recv(asock, buffer, 255, MSG_DONTWAIT);
		if(msglen > 0){
			printf("Message Received: %s", buffer);
			send_handler(buffer);
		}
	}	
}

void error(char *msg){
	perror(msg);
	exit(1);
}

int main(void){
	int sock, asock, client_len, msglen;
  struct sockaddr_in server, client;
	char buffer[256];
	int port = 6000;

	n_clients = 0;
	key = 5963;
	shm_size = (MAX_CLIENTS + 1) * sizeof(int);

	if((shmid = shmget(key, shm_size, IPC_CREAT | 0666)) < 0){
		perror("Shmget Failed: ");
		exit(1);
	}

	if((shm = shmat(shmid, NULL, 0)) == (char *)-1){
		perror("Shmat Failed: ");
		exit(1);
	}

	all_clients = (int *)shm;
	memset(all_clients, 0, (MAX_CLIENTS + 1) * sizeof(int));


	bzero(buffer, 256);
	bzero((char *) &server, sizeof(server));	
	bzero((char *) &client, sizeof(client));	

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0)
		error("Socket Failed: ");

	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = INADDR_ANY;

	if(bind(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
		error("Bind Failed: ");
	
	if(listen(sock, 5) < 0)
		error("Listen Failed: ");

	client_len = sizeof(client);

	while((asock = accept(sock, (struct sockaddr *)&client, &client_len)) > 0){
		if(asock < 0)
			error("Accept Failed: ");

		*(all_clients + n_clients * sizeof(int)) = asock;
		n_clients++;
		
		if(fork() != 0){
			/* int shmid; */
			/* key_t key = ; */
			/* char *shm; */
			/* int *all_clients; */
			/* printf("asock: %d\n", asock); */
			printf("all_clients: ");
			for(int i = 0; i < n_clients; i++){
				printf("%d, ", *(all_clients + i * sizeof(int)));
			}
			printf("\n");
			/* recv_handler(asock); */

			/* bzero(buffer, 256); */
			/* strcpy(buffer, "This is server.\n"); */
			/* msglen = send(asock, buffer, strlen(buffer), 0); */
			/* printf("Sent: %s\t%d\n", buffer, msglen); */
			close(asock);
		}

		printf("n_clients: %d\n", n_clients);
	}

	shmdt(shm);
	close(sock);
	return 0;
}