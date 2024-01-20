#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>


#define MAX 80
#define PORT 8080
#define SERVER_ADDRESS  "127.0.0.1"
#define FILENAME_A        "anonimo.txt"
#define FILENAME_R        "registrato.txt"


void open_file(int);
void receive(int);
