#include "server_dtp.h"

void send_file(int sockfd, int connfd, struct sockaddr_in servaddr, struct sockaddr_in cli, int id){

  socklen_t sock_len;
  ssize_t len;
  int sent_bytes = 0;
  FILE *fd;
  off_t offset;
  int remain_data;
  struct stat file_stat;
  char file_size [256];
  char buff [MAX];

  printf("send_file - procedure\n");

  if (id == 1){
    fd = (fopen(R_FILE, "r"));
  } else if (id == 2){
    fd = (fopen(A_FILE, "r"));
  }

  printf("File opened\n");

  if (fd == NULL){
    fprintf(stderr, "Error opening file --> %s", strerror(errno));
    exit(0);
  }

  printf("Fstat - procedure\n");

  /* Get file stats */
  if (fstat(fileno(fd), &file_stat) < 0){
    fprintf(stderr, "Error fstat --> %s", strerror(errno));
    exit(EXIT_FAILURE);
  }

  fprintf(stdout, "File Size: \n%ld bytes\n", file_stat.st_size);

  sock_len = sizeof(cli);
  /* Accepting incoming peers */


  /*
  connfd = accept(sockfd, (struct sockaddr *)&cli, &sock_len);
  if (connfd == -1){
    fprintf(stderr, "Error on accept --> %s", strerror(errno));
    exit(EXIT_FAILURE);
  }*/
  fprintf(stdout, "Accept peer --> %s\n", inet_ntoa(cli.sin_addr));
  sprintf(file_size, "%ld", file_stat.st_size);

  /* Sending file size */
  len = send(connfd, file_size, sizeof(file_size), 0);
  if (len < 0){
    fprintf(stderr, "Error on sending greetings --> %s", strerror(errno));
    exit(0);
  }

  fprintf(stdout, "Server sent %ld bytes for the size\n", len);

  offset = 0;
  remain_data = file_stat.st_size;
  /* Sending file data */
  while (((sent_bytes = sendfile(connfd, fileno(fd), &offset, sizeof(buff))) > 0) && (remain_data > 0)){
    remain_data -= sent_bytes;
    fprintf(stdout, "1. Server sent %d bytes from file's data, offset is now : %ld and remaining data = %d\n", sent_bytes, offset, remain_data);
  }

}
