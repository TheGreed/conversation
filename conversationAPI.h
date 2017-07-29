#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>
#include <crypt.h>
#include <unistd.h>

static char *host = "127.0.0.1";
static char *user = "root";
static char *pass = "admin";
static char *dbname = "conversation";

const char *salt = "$6$a9$";

unsigned int port = 3306;
unsigned int flag = 0;

static char *unix_socket = NULL;

/* returns user_id if user is valid and -1 if user is invalid */
int isUser(MYSQL *conn, char *username);

/* returns 0 on success and -1 on failure to add */
int addFriend(MYSQL *conn, char *user1, char *user2);

/* returns user_id of user on success and -1 on failure */
int login(MYSQL *conn, char *username, char *password);

/* creates MySQL connection */
MYSQL * converInit();

/* returns encrypted password on success and NULL on error */
char *getEncpass(char *password);
