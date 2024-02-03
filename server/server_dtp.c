#include "server_dtp.h"

void send_file(int sockfd, int connfd, struct sockaddr_in servaddr, struct sockaddr_in cli, char *file){

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

  sock_len = sizeof(cli);

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


void receive_file (int connfd, char *file){

  char buff[MAX];
  char c;
  FILE *received_file;
  int remain_data = 0;
  int file_size;
  ssize_t len;

  

  recv(connfd, buff, sizeof(buff), 0);
  file_size = atoi(buff);

  fprintf(stdout, "\nFile size : %d\n", file_size);

  received_file = fopen(file, "w");

  

  if (received_file == NULL){
    fprintf(stderr, "Failed to open file --> %s\n", strerror(errno));

    exit(EXIT_FAILURE);
  }



  remain_data = file_size;
  while ((remain_data > 0) && ((len = recv(connfd, buff, sizeof(buff), 0)) > 0)){
    fwrite(buff, sizeof(char), len, received_file);
    remain_data -= len;
    fprintf(stdout, "Receive %ld bytes and we hope :- %d bytes\n", len, remain_data);
  }
  fclose(received_file);
  edit_rg(file);
}

void edit_file_name(char * old_file_name, char *new_file_name){

  char c_name[MAX];

  FILE *fd = fopen("rg.txt", "r+");
  FILE *f_temp = fopen("temp.txt", "w");
  if (fd == NULL || f_temp == NULL){
    fprintf(stderr, "Failed to open file --> %s\n", strerror(errno));
    exit(0);
  }

 
  while (fscanf(fd, "%s" , c_name) != EOF){
   
    if (strncmp(c_name, old_file_name, sizeof(old_file_name)) == 0){
      
      if (rename(old_file_name, new_file_name) == 0)
        printf("Name changed into %s\n", new_file_name);
      else{
        fprintf(stderr, "Error --> %s\n", strerror(errno));
        exit(0);
      }
      fprintf(f_temp, "%s\n", new_file_name);
    } else
      fprintf(f_temp, "%s\n", c_name);
  }
 
 

  fclose(fd);
  fclose(f_temp);

  remove("rg.txt");
  rename("temp.txt", "rg.txt");
}

void edit_rg (char *file_name){

  FILE *fd;

  fd = fopen("rg.txt", "a");
  if (fd == NULL){
    fprintf(stderr, "Failed to open file --> %s\n", strerror(errno));
    exit(0);
  }

  fprintf(fd, "%s" , file_name);
  fclose(fd);
}
