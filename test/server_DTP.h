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


#define PORT_NUMBER     8080
#define SERVER_ADDRESS  "127.0.0.1"
#define FILE_TO_SEND_A  "anonimo.txt"
#define FILE_TO_SEND_R  "registrato.txt"
#define MAX 80

void send(int, int, struct sockaddr)
