#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h> // read(), write(), close()

#include "server_dtp.h"

#define MAX 256
#define PORT 8080
#define SA struct sockaddr

// Function designed for the login phase
int login(int connfd){

	char buff[MAX];
	int check = 0;

	bzero(buff, MAX);

	// Joining choice
	strcpy(buff, "*** WELCOME ***\n\t1 - Login\n\t2 - Anonymous");
	write(connfd, buff, sizeof(buff));
	bzero(buff, MAX);

	// Choice check
	do{
		read(connfd, buff, sizeof(buff));
		if (strncmp(buff, "1", 1) == 0){
			bzero(buff, MAX);
			strcpy(buff, "\tpassword: ");
			write(connfd, buff, sizeof(buff));
			bzero(buff, MAX);
			read(connfd, buff, sizeof(buff));
			if (strncmp(buff, "admin", 5) == 0){
				bzero(buff, MAX);
				strcpy(buff, "*** BENTORNATO ***\nPress <Enter> to continue");
				write(connfd, buff, sizeof(buff));
				bzero(buff, MAX);
			}else{
				bzero(buff, MAX);
				strcpy(buff, "*** ERRORE - Wrong Password ***");
				write(connfd, buff, sizeof(buff));
				bzero(buff, MAX);
				check = 0;
			}
			return 1;
		}	else if (strncmp(buff, "2", 1) == 0){
			bzero(buff, MAX);
			strcpy(buff, "*** BENVENUTO ***\nPress <Enter> to continue");
			write(connfd, buff, sizeof(buff));
			bzero(buff, MAX);
			return 2;
		} else {
			bzero(buff, MAX);
			strcpy(buff, "*** ERRORE - Wrong choice ***");
			write(connfd, buff, sizeof(buff));
			bzero(buff, MAX);
			check = 0;
		}
	}while(check == 0);
}

void send_info(int connfd, int id){

	char buff [MAX];
	read(connfd, buff, sizeof(buff));
	bzero(buff, MAX);
	if (id == 1)
		strcpy(buff, "1");
	else if (id == 2)
		strcpy(buff, "2");
	write(connfd, buff, sizeof(buff));
	bzero(buff, MAX);
}

// Function designed to manage the client choices
void op_choice (int sockfd, int connfd, struct sockaddr_in servaddr, struct sockaddr_in cli, int id){

	char buff[MAX];
	char *op;
        
	if (id == 1){
	
		bzero(buff, MAX);
		strcpy(buff, "\t1 - Download\n\t2 - Upload\n\t3 - Rename\n\t0 - Exit\n");
		write(connfd, buff, sizeof(buff));

		bzero(buff, MAX);
		read(connfd, buff, sizeof(buff));

		if (strncmp(buff, "1", 1) == 0){

			bzero(buff, MAX);
			strcpy(buff, "\tInsert file name:\n");
			write(connfd, buff, sizeof(buff));

			bzero(buff, MAX);
			read(connfd, buff, sizeof(buff));

			char fc[256];
			strcpy(fc,buff);
			fc[strcspn(fc,"\r\n")] = 0;

			bzero(buff, MAX);
			strcpy(buff, "r");
			write(connfd, buff, sizeof(buff));


			bzero(buff, MAX);
			send_file(sockfd, connfd, servaddr, cli, fc);

		} else if (strncmp(buff, "2", 1) == 0){

			bzero(buff, MAX);
			strcpy(buff, "\tInsert file name:\n");
			write(connfd, buff, sizeof(buff));

			bzero(buff, MAX);
			read(connfd, buff, sizeof(buff));

			char fc[256];
			strcpy(fc,buff);
			fc[strcspn(fc,"\r\n")] = 0;

			bzero(buff, MAX);
			strcpy(buff, "w");
			write(connfd, buff, sizeof(buff));


			bzero(buff, MAX);
			receive_file(connfd, fc);

		} else if (strncmp(buff, "3", 1) == 0){

			char o_name[MAX];
			char n_name[MAX];

			bzero(buff, MAX);
			strcpy(buff, "\tInsert the old file name:\n");
			write(connfd, buff, sizeof(buff));

			bzero(buff, MAX);
			read(connfd, buff, sizeof(buff));

			strcpy(o_name, buff);
			o_name[strcspn(o_name,"\r\n")] = 0;

			bzero(buff, MAX);
			strcpy(buff, "\tInsert the new file name:\n");
			write(connfd, buff, sizeof(buff));

			bzero(buff, MAX);
			read(connfd, buff, sizeof(buff));

			strcpy(n_name, buff);
			n_name[strcspn(n_name,"\r\n")] = 0;

			edit_file_name(o_name, n_name);

			bzero(buff, MAX);
			strcpy(buff, "\tFile name changed\n");
			write(connfd, buff, sizeof(buff));

			bzero(buff, MAX);

		} else if (strncmp(buff, "0", 1) == 0){

			bzero(buff, MAX);
			strcpy(buff, "exit");
			write(connfd, buff, sizeof(buff));
			bzero(buff, MAX);
			
			strcpy(buff,R_FILE);
			write(connfd, buff, sizeof(buff));
			bzero(buff, MAX);

			printf("Server Exit...\n");
			while(1)
			break;

		}
	
	} else if (id == 2){
	
		bzero(buff, MAX);
		strcpy(buff, "\t1 - Download\n\t0 - Exit\n");
		write(connfd, buff, sizeof(buff));

		bzero(buff, MAX);
		read(connfd, buff, sizeof(buff));

		if (strncmp(buff, "1", 1) == 0){

			bzero(buff, MAX);
			strcpy(buff, "\tInsert file name:\n");
			write(connfd, buff, sizeof(buff));

			bzero(buff, MAX);
			read(connfd, buff, sizeof(buff));

			char fc[256];
			strcpy(fc,buff);
			fc[strcspn(fc,"\r\n")] = 0;

			bzero(buff, MAX);
			strcpy(buff, "r");
			write(connfd, buff, sizeof(buff));

			bzero(buff, MAX);
			send_file(sockfd, connfd, servaddr, cli, fc);

		} else if (strncmp(buff, "0", 1) == 0){

			bzero(buff, MAX);
			strcpy(buff, "exit");
			write(connfd, buff, sizeof(buff));
			bzero(buff, MAX);
			
			strcpy(buff,A_FILE);
			write(connfd, buff, sizeof(buff));
			bzero(buff, MAX);

			printf("Server Exit...\n");
			while(1)
			break;

		}
	
	
	}
	sleep(1);

}



// Driver function
int main(){
	int sockfd, connfd, len;
	char *file;
	struct sockaddr_in servaddr, cli;

	// socket create and verification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created..\n");
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	// Binding newly created socket to given IP and verification
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
		printf("socket bind failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully binded..\n");

	// Now server is ready to listen and verification
	if ((listen(sockfd, 5)) != 0) {
		printf("Listen failed...\n");
		exit(0);
	}
	else
		printf("Server listening..\n");
	len = sizeof(cli);

	// Accept the data packet from client and verification
	connfd = accept(sockfd, (SA*)&cli, &len);
	if (connfd < 0) {
		printf("server accept failed...\n");
		exit(0);
	}
	else
		printf("server accept the client...\n");

	// Login phase
	int log = login(connfd);
	printf("Login ended with value: %d\n", log);

	send_info(connfd, log);
	printf("The info are sent\n");

	if (log == 1)
		send_file(sockfd, connfd, servaddr, cli, R_FILE);
	else if (log == 2)
		send_file(sockfd, connfd, servaddr, cli, A_FILE);
		
        while(1)
	op_choice(sockfd, connfd, servaddr, cli, log);
	close(sockfd);
}
