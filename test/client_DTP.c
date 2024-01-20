


#include "client_DTP.h"


void open_file(int id){
if(id==1){
   FILE *file;
   file = fopen(FILENAME_R,"r");
   while(file.feof()){
   fprintf(file,"\n");
   }
   fclose(file);
 } else if(id==2){
     FILE *file;
   file = fopen(FILENAME_A,"r");
   while(file.feof()){
   fprintf(file,"\n");
   }
   fclose(file);
 }
}


void receive(int sockfd)
{
        ssize_t len;       
        char buffer[MAX];
        int file_size;
        FILE *received_file;
        int remain_data = 0;

        /* Receiving file size */
        recv(sockfd, buffer, MAX, 0);
        file_size = atoi(buffer);
        //fprintf(stdout, "\nFile size : %d\n", file_size);

        received_file = fopen(FILENAME, "w");
        if (received_file == NULL)
        {
                fprintf(stderr, "Failed to open file foo --> %s\n", strerror(errno));

                exit(EXIT_FAILURE);
        }

        remain_data = file_size;

        while ((remain_data > 0) && ((len = recv(sockfd, buffer, MAX, 0)) > 0))
        {
                fwrite(buffer, sizeof(char), len, received_file);
                remain_data -= len;
                fprintf(stdout, "Receive %d bytes and we hope :- %d bytes\n", len, remain_data);
        }
        fclose(received_file);

        

        
}





