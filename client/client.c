#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // bzero()
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()

#include <sys/wait.h>

#include "client_dtp.h"

#define MAX 256
#define PORT 8080
#define SA struct sockaddr

void func(int sockfd, struct sockaddr_in servaddr){
	char buff[MAX];
	char fc[MAX];
	char *file;
	int n;
	for (;;) {
		bzero(buff, MAX);		//bzero(buff, sizeof(buff));
		read(sockfd, buff, sizeof(buff));

		if (strncmp(buff, "w", 1) == 0){
			send_file(sockfd, servaddr, fc);
			bzero(buff, sizeof(buff));
		} else if (strncmp(buff, "r", 1) == 0){
			receive_file (sockfd, fc);
			bzero(buff, sizeof(buff));
		}

		if (strncmp(buff, "1", 1) == 0){
			receive_file (sockfd , R_FILE);
			bzero(buff, sizeof(buff));
			read(sockfd, buff, sizeof(buff));
		} else if (strncmp(buff, "2", 1) == 0){
			receive_file (sockfd, A_FILE);
			bzero(buff, sizeof(buff));
			read(sockfd, buff, sizeof(buff));
		} else if ((strncmp(buff, "exit", 4)) == 0) {

			bzero(buff,sizeof(buff));
		        read(sockfd, buff, sizeof(buff));
		        remove(buff);
		        bzero(buff,sizeof(buff));
			
			printf("Client Exit...\n");
			break;
		} else
			printf("Server :\n %s", buff);


		bzero(buff, sizeof(buff));
		printf("\n> ");
		n = 0;
		while ((buff[n++] = getchar()) != '\n');
		write(sockfd, buff, sizeof(buff));
		strcpy(fc,buff);
		// Check for text
		if (strlen(buff) > 4){
			file = &buff[strlen(buff) - 5];
			if (strncmp(file, ".txt", 4) == 0){
				fc[strcspn(fc,"\r\n")] = 0;
				
			}
		}
		bzero(buff, MAX);
	}
}

int main(){

	int sockfd, connfd;
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
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);

	// connect the client socket to server socket
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr))
		!= 0) {
		printf("connection with the server failed...\n");
		exit(0);
	}
	else
		printf("connected to the server..\n");

	// function for chat
	func(sockfd, servaddr);

	// close the socket
	close(sockfd);
}
