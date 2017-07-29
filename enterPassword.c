#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <crypt.h>

const char *username;
const char *password;
const char *encpass;
const char *conversation;

/* ideally this function should return if it was successful or not in reading the input, if not program should return EXIT_FAILURE */
int readArg(int argc, const char *argv[]){

	/* loop goes through all the arguments given from the terminal */
	/* except the first argument because that is just the program name */
	for(int i = 1; i < argc; i++){
		
		/* argument at i is an option argument */
		if(*(argv[i]) == '-'){
			/* is the next char of the option a u for username? */
			if(*(argv[i]+1) == 'u'){
				/* username option picked, but is teh username in the next option or the same option with the -u */
				if(argc > (i+1) && *(argv[i+1]) == '-'){
					/* set username to point to argv[i] which is the "-uUsername", but to get rid of the -u I add 2 to the username pointer */
					username = argv[i];
					username += 2;
				}else{
					/* username is the next argument so I just set it to that */
					username = argv[i+++1];
				}
			}
			/* is option password -p set? */
			else if(*(argv[i]+1) == 'p'){
				/* if the argument with the -p option is longer than 2 then the rest of the characters are teh password */
				if(strlen(argv[i]) > 2){
					password = argv[i];
					password += 2;
				}
				/* password is not in the same argument, so it must be the next argument or password prompt must show up */
				else{
					if(argc > (i+1) && *(argv[i+1]) != '-'){
						password = argv[i+1];
					}else{
						password = getpass("Password: ");
					}
				}
			}
		}
	}

	/* if username or password is null then the right arguments were not sent */ 
	/* return 1 for failure because username and password were not set */
	if(!(username || password)){
		return 1;
	}

	/* return success */
	return 0;
}

int main(int argc, const char *argv[]){
	
	/* on success we may encrypt password */
	if(!readArg(argc, argv)){
		
		/* crypt is needs headers unistd.h and crypt.h */ 
		/* the first number between the dolar signs is the encryption type: */ 
		/* 6 is SHA-512 and the next string between dolar signs is the salt */
		encpass = crypt(password, "$6$a9$");

		/* not sure if necessary or correct, but moving pointer 6 foward because everystring shows the "$6$a9$" */ 
		/* string in front of the encrypted password */
		/* encpass = encpass+6; */

		/* printing to show it worked */
		printf("username: %s\n", username);
		printf("password: %s\n", password);
		printf("encpass: %s\n", encpass);
		printf("enchpass length: %lu\n", strlen(encpass));

	}

	return EXIT_SUCCESS;
}
