#include "client_dtp.h"

void receive_file (int sockfd, int id, struct sockaddr_in servaddr){

  char buff[MAX];
  char c;
  FILE *received_file;
  int remain_data = 0;
  int file_size;
  ssize_t len;

/*
  if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(struct sockaddr)) == -1){
    fprintf(stderr, "Error on connect --> %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }*/


  printf("receive_file - procedure");

  recv(sockfd, buff, sizeof(buff), 0);
  file_size = atoi(buff);

  fprintf(stdout, "\nFile size : %d\n", file_size);

  if (id == 1)
    received_file = fopen(R_FILE, "w");
  else if (id == 2)
    received_file = fopen(A_FILE, "w");

  printf("File opened\n");

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
  read_file(2);
}

void read_file (int id){

  FILE *f;
  char c;

  if (id == 1){
    f = (fopen(R_FILE, "r"));
    if (f == NULL){
      printf("Cannot open the file...\n");
      exit (0);
    }
  } else if (id == 2){
    f = (fopen(A_FILE, "r"));
    if (f == NULL){
      printf("Cannot open the file...\n");
      exit (0);
    }
  }

  c =  fgetc(f);
  while (c != EOF){
    printf("%c", c);
    c =  fgetc(f);
  }

  fclose(f);
}
