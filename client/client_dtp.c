#include "client_dtp.h"

void receive_file (int sockfd, char *file){

  char buff[MAX];
  char c;
  FILE *received_file;
  int remain_data = 0;
  int file_size;
  ssize_t len;

  

  recv(sockfd, buff, sizeof(buff), 0);
  file_size = atoi(buff);

  fprintf(stdout, "\nFile size : %d\n", file_size);

  received_file = fopen(file, "w");

  
  if (received_file == NULL){
    fprintf(stderr, "Failed to open file --> %s\n", strerror(errno));

    exit(EXIT_FAILURE);
  }



  remain_data = file_size;
  while ((remain_data > 0) && ((len = recv(sockfd, buff, sizeof(buff), 0)) > 0)){
    fwrite(buff, sizeof(char), len, received_file);
    remain_data -= len;
    fprintf(stdout, "Receive %ld bytes and we hope :- %d bytes\n", len, remain_data);
  }
  fclose(received_file);
  read_file(file);
}

void send_file(int sockfd, struct sockaddr_in servaddr, char *file){

  socklen_t sock_len;
  ssize_t len;
  int sent_bytes = 0;
  FILE *fd;
  off_t offset;
  int remain_data;
  struct stat file_stat;
  char file_size [256];
  char buff [MAX];


  fd = (fopen(file, "r"));

 

  if (fd == NULL){
    fprintf(stderr, "Error opening file --> %s", strerror(errno));
    exit(0);
  }

  

  /* Get file stats */
  if (fstat(fileno(fd), &file_stat) < 0){
    fprintf(stderr, "Error fstat --> %s", strerror(errno));
    exit(EXIT_FAILURE);
  }

  fprintf(stdout, "File Size: \n%ld bytes\n", file_stat.st_size);

  sock_len = sizeof(servaddr);

  //fprintf(stdout, "Accept peer --> %s\n", inet_ntoa(cli.sin_addr));
  sprintf(file_size, "%ld", file_stat.st_size);

  /* Sending file size */
  len = send(sockfd, file_size, sizeof(file_size), 0);
  if (len < 0){
    fprintf(stderr, "Error on sending greetings --> %s", strerror(errno));
    exit(0);
  }

  fprintf(stdout, "Server sent %ld bytes for the size\n", len);

  offset = 0;
  remain_data = file_stat.st_size;
  /* Sending file data */
  while (((sent_bytes = sendfile(sockfd, fileno(fd), &offset, sizeof(buff))) > 0) && (remain_data > 0)){
    remain_data -= sent_bytes;
    fprintf(stdout, "1. Server sent %d bytes from file's data, offset is now : %ld and remaining data = %d\n", sent_bytes, offset, remain_data);
  }
   
}

void read_file (char *file){

  FILE *f;
  char c;

  f = fopen(file, "r");
  if (f == NULL){
    printf("Cannot open the file...\n");
    exit (0);
  }

  c =  fgetc(f);
  while (c != EOF){
    printf("%c", c);
    c =  fgetc(f);
  }

  fclose(f);
}
