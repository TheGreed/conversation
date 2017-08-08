#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <ncurses.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int sock;
int *shm_sock;
key_t key;
int shmid;
int shm_size;
char *shm;

WINDOW *user_input;
WINDOW *chat_log;

NCURSES_WINDOW_CB chat_handler(){
	/* int sock = *(int *)s; */
	int sock = *shm_sock;
	char *chat_buff = malloc(256 * sizeof(char));
	int msglen;

	mvwprintw(chat_log, 10, 10, "Hello World");
	wrefresh(chat_log);

	while(1){
		bzero(chat_buff, 256 * sizeof(char));
		msglen = recv(sock, chat_buff, 255, 0);
		if(msglen < 0){
			perror("Received Failed: ");
			close(sock);
			delwin(chat_log);
			delwin(user_input);
			endwin();
			exit(1);
		}

		/* scroll(chat_log); */
		mvwprintw(chat_log, 10, 0, chat_buff);
		wrefresh(chat_log);
	}
}

NCURSES_WINDOW_CB input(){
	/* int sock = *(int *)s; */
	/* int sock = 3; */
	int sock = *shm_sock;
	/* mvwprintw(chat_log, 5, 4, "sock: %d", *shm_sock); */
	/* wrefresh(chat_log); */
	char buffer[256];
	int msglen;

	/* mvwprintw(user_input, 0, 10, "Hello"); */
	/* wrefresh(user_input); */

	while(1){
		bzero(buffer, 256);
		wgetstr(user_input, buffer);	
		mvwprintw(chat_log, 1, 1, buffer);
		wrefresh(chat_log);
		msglen = send(sock, buffer, 255, 0);
		if(msglen < 0){
			perror("Send Failed: ");
			close(sock);
			delwin(user_input);
			delwin(chat_log);
			endwin();
			exit(1);
		}
		/* mvwprintw(chat_log, 2, 1, "%d", msglen); */
		wclear(user_input);
		wrefresh(user_input);
	}
}

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

	initscr();
	chat_log = newwin(0, 0, 0, 0);
	user_input = newwin(1, 0, LINES - 1, 0);
	wresize(chat_log, LINES - 2, COLS);
	curs_set(0);
	scrollok(chat_log, 1);

	key = 8090;
	shm_size = 2 * sizeof(int);

	if((shmid = shmget(key, shm_size, IPC_CREAT | 0666)) < 0){
		perror("Shmget Failed: ");
		exit(1);
	}

	if((shm = shmat(shmid, NULL, 0)) == (char *)-1){
		perror("Shmat Failed: ");
		exit(1);
	}

	shm_sock = (int *)shm;
	bzero(shm_sock, 2 * sizeof(int));

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

	*shm_sock = sock;

	mvwprintw(chat_log, 4, 4, "sock: %d", *shm_sock);
	wrefresh(chat_log);

	/* int *sock_ptr = &sock; */
	/* mvwprintw(chat_log, 4, 4, "sock: %d", *sock_ptr); */
	/* wrefresh(chat_log); */

	use_window(user_input, (NCURSES_WINDOW_CB)input, NULL);
	use_window(chat_log, (NCURSES_WINDOW_CB)chat_handler, NULL);

	/* while(1){ */
		
	/* } */
	
	/* pthread_t chat; */
	/* pthread_create(&chat, NULL, *chat_handler, (void *)&sock); */
	/* if(fork() > 0){ */
	/* 	chat_handler(sock); */	
	/* } */

	/* while(1){ */
	/* 	bzero(buffer, 256); */
	/* 	wgetstr(user_input, buffer); */	
	/* 	wclear(user_input); */
	/* 	send(sock, buffer, 255, 0); */
	/* 	wrefresh(stdscr); */
	/* 	wrefresh(user_input); */
	/* } */

	/* if(fork() > 0){ */
	/* 	while(1){ */
	/* 		bzero(buffer, 256); */
	/* 		strcpy(buffer, "This is client.\n"); */
	/* 		send(sock, buffer, 255, 0); */
	/* 		sleep(1); */
	/* 	} */
	/* } */	
	/* while(1){ */
	/* 	msglen = recv(sock, buffer, 255, 0); */
	/* 	if(msglen < 0){ */
	/* 		perror("Recieved Failed: "); */
	/* 		close(sock); */
	/* 		exit(1); */
	/* 	} */

	/* 	printf("Buffer: %s", buffer); */
	/* } */

/* 		bzero(buffer, 256); */
/* 		if((msglen = recv(sock, buffer, 256, 0)) < 0){ */
/* 			perror("Receive Failed: "); */
/* 			close(sock); */
/* 			exit(1); */
/* 		} */
/* 		msglen = 5; */
/* 		printf("Received from server: %s\t%d", buffer, msglen); */
/* 		printf("msglen: %d\n", msglen); */

		/* sleep(1); */

	shmdt(shm);
	delwin(chat_log);
	delwin(user_input);
	endwin();
	close(sock);
	return 0;
}
