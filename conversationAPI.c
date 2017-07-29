#include "conversationMySQLAPI.h"

MYSQL *converInit(){
	MYSQL *conn = mysql_init(NULL);
	if(!mysql_real_connect(conn, host, user, pass, dbname,
												 port, unix_socket, flag))
	{
		fprintf(stderr, "\nError: %s [%d]\n", mysql_error(conn), 
																				mysql_errno(conn));
		exit(1);
	}
	return conn;
}

char *getEncpass(char *password){
	return crypt(password, salt);
}

int isUser(MYSQL *conn, char *username){
	char query[200] = "SELECT user_id FROM users WHERE username='";	
	strcat(query, username);
	strcat(query, "'");

	if(mysql_query(conn, query)){
		fprintf(stderr, "\nError: %s [%d]\n", mysql_error(conn), 
																				mysql_errno(conn));
		exit(1);
	}

	MYSQL_RES *result = mysql_use_result(conn);
	MYSQL_ROW row;

	/* on success return user_id */
	if((row = mysql_fetch_row(result))){
		int user_id = **row;
		mysql_free_result(result);
		return user_id;
	}

	/* on failure return -1 */
	mysql_free_result(result);
	return -1;
}

int login(MYSQL *conn, char *username, char *password){
	char query[200] = "SELECT user_id FROM users WHERE username ='";
	strcat(query, username);
	strcat(query, "' AND password='");
	strcat(query, password);
	strcat(query, "'");

	if(mysql_query(conn, query)){
		fprintf(stderr, "\nError: %s [%d]\n", mysql_error(conn), 
																				mysql_errno(conn));
		exit(1);
	}
	
	MYSQL_RES *result = mysql_use_result(conn);
	MYSQL_ROW row;

	/* on success return user_id */
	if((row = mysql_fetch_row(result))){
		int user_id = **row;
		mysql_free_result(result);
		return user_id;
	}

	/* on failure return -1 */
	mysql_free_result(result);
	return -1;
}

int addFriend(MYSQL *conn, char *user1, char *user2){
	/* assuming user1 and user2 are valid users */
	char query[200] = "INSERT INTO relationship (user1, user2, status) VALUES ('";
	if(strcmp(user1, user2) < 0){
		strcat(query, user1);
		strcat(query, "', '");
		strcat(query, user2);
		strcat(query, "', '1')");
	}else if(strcmp(user1, user2) > 0){
		strcat(query, user2);
		strcat(query, "', '");
		strcat(query, user1);
		strcat(query, "', '1')");
	}else{
		return -1;
	}

	if(mysql_query(conn, query)){
		fprintf(stderr, "\nError: %s [%d]\n", mysql_error(conn), 
																				mysql_errno(conn));
		exit(1);
	}
	
	return 0;
}

int main(void){
	printf("Hello World\n");	
	return 0;
}
