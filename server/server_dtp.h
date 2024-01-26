#ifndef _SERVER_DTP_H_
#define _SERVER_DTP_H_

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sendfile.h>

#define MAX 256
#define PORT 8080
#define A_FILE "an.txt"
#define R_FILE "rg.txt"

void receive_file ();
void send_file(int , int , struct sockaddr_in , struct sockaddr_in , int );
void edit_file_name(int ); // exec for the dir_file | fopen for the content

#endif // _SERVER_DTP_H_
