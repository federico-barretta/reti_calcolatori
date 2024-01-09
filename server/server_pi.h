#ifndef SERVER_PI
#define SERVER_PI

#include  <stdio.h>
#include  <string.h>
#include  <sys/socket.h>
#include  <sys/stat.h>
#include  <sys/sendfile.h>
#include  <fcntl.h>
#include  <arpa/inet.h>
#include  <unistd.h>
#include  <stdlib.h>
#include  <stdbool.h>
#include  <pthread.h>

#define SERVER_PORT  8080

int socket_id;
int socket_client;

struct sockaddr_in server;
struct sockaddr_in client;






#endif
