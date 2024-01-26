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

	// 1 - Login
	// 2 - Anonymous

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

// Function designed for exchange message
void func(int connfd){

	char buff[MAX];
	int n;
	int counter_message = 0;
	// infinite loop for chat
	for (;;) {
		bzero(buff, MAX);

		// read the message from client and copy it in buffer
		read(connfd, buff, sizeof(buff));
		// print buffer which contains the client contents
		printf("From client: %s\t To client : ", buff);
		//bzero(buff, MAX);
		//n = 0;

		// copy server message in the buffer
		//while ((buff[n++] = getchar()) != '\n');

    counter_message += 1;

		// and send that buffer to client
		write(connfd, buff, sizeof(buff));

		// if msg contains "Exit" then server exit and chat ended.
		if (strncmp("exit", buff, 4) == 0) {
			printf("Server Exit...\n");
			break;
		}
	}
}

// Driver function
int main(){
	int sockfd, connfd, len;
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
	send_file(sockfd, connfd, servaddr, cli, log);
	// Send 1/2 to Client
	// Send an.txt/rg.txt to client
	// Receive choice for download
	// Send chosen file

	// Function for exchange message between client and server
	func(connfd);

	// After chatting close the socket
	close(sockfd);
}
