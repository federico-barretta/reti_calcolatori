#ifndef _CLIENT_DTP_H_
#define _CLIENT_DTP_H_

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>

#define MAX 256
#define PORT 8080
#define A_FILE "an.txt"
#define R_FILE "rg.txt"

void read_file (int );
void receive_file (int , int , struct sockaddr_in );
void send_file();
void edit_file_name(); // (?)

#endif // _CLIENT_DTP_H_
