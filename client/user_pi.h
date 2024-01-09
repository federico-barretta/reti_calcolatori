#ifndef USER_PI
#define USER_PI

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 2500

struct sockaddr_in server;
int socket_id;

char richiesta [BUFSIZ];
char risposta  [BUFSIZ];




#endif
