

#include "server_DTP.h"



void send(int sockfd,int connfd,struct sockaddr_in cli)
{
     
        socklen_t       sock_len;
        ssize_t len;
     
        int fd;
        int sent_bytes = 0;
        char file_size[256];
        struct stat file_stat;
        int offset;
        int remain_data;

      

       


      

        fd = open(FILE_TO_SEND, O_RDONLY);
        if (fd == -1)
        {
                fprintf(stderr, "Error opening file --> %s", strerror(errno));

                exit(EXIT_FAILURE);
        }

        /* Get file stats */
        if (fstat(fd, &file_stat) < 0)
        {
                fprintf(stderr, "Error fstat --> %s", strerror(errno));

                exit(EXIT_FAILURE);
        }

        fprintf(stdout, "File Size: \n%d bytes\n", file_stat.st_size);

        sock_len = sizeof(struct sockaddr_in);
        /* Accepting incoming peers */
        connfd = accept(sockfd, (struct sockaddr *)&cli, &sock_len);
        if (connfd == -1)
        {
                fprintf(stderr, "Error on accept --> %s", strerror(errno));

                exit(EXIT_FAILURE);
        }
        fprintf(stdout, "Accept peer --> %s\n", inet_ntoa(cli.sin_addr));

        sprintf(file_size, "%d", file_stat.st_size);

        /* Sending file size */
        len = send(connfd, file_size, sizeof(file_size), 0);
        if (len < 0)
        {
              fprintf(stderr, "Error on sending greetings --> %s", strerror(errno));

              exit(EXIT_FAILURE);
        }

        fprintf(stdout, "Server sent %d bytes for the size\n", len);

        offset = 0;
        remain_data = file_stat.st_size;
        /* Sending file data */
        while (((sent_bytes = sendfile(connfd, fd, &offset, MAX)) > 0) && (remain_data > 0))
        {
                fprintf(stdout, "1. Server sent %d bytes from file's data, offset is now : %d and remaining data = %d\n", sent_bytes, offset, remain_data);
                remain_data -= sent_bytes;
                fprintf(stdout, "2. Server sent %d bytes from file's data, offset is now : %d and remaining data = %d\n", sent_bytes, offset, remain_data);
        }

        
         
}
