#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <fcntl.h>

#define SOCK_PATH "santa_monica"
#define BUFFER_SIZE 48
char *EOF_path = "/tmp/EOF_pipe";
int main()
{
    char readbuff[BUFFER_SIZE];
    int loc = 0;
    int sockfd;
    // mkfifo(EOF_path, 0666);
    // int EOF_pipe = open(EOF_path, O_WRONLY);
    struct sockaddr_un serv_addr;
    // char buffer[256];
    sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sockfd < 0)
        perror("ERROR IN TURNING ON THE TV\n");
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path, SOCK_PATH);
    FILE *fptr = fopen("./tayloremipsum", "r");
    printf("CONCERT STARTED...\n");
    char buff[10];
    int n;
    int iter = 0;
    while (1)
    {
        printf("\r");
        iter++;
        bzero(readbuff, BUFFER_SIZE);
        n = fread(readbuff, sizeof(char), BUFFER_SIZE - 1, fptr);
        readbuff[BUFFER_SIZE - 1] = '\0';

        n = sendto(sockfd, readbuff, strlen(readbuff), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

        if (n < 0)
        {
            perror("\nCONCERT ERROR!!\n");
            break;
        }
        if (n == 0)
        {
            printf("\nCONCERT ENDED!!\n");
            break;
        }
        // printf(readbuff);
        if (iter % 3 == 0)
        {
            printf("writing.");
        }
        else if (iter % 3 == 1)
        {
            printf("writing...");
        }
        else
        {
            printf("writing....");
        }
        fflush(stdout);
        sleep(1);
    }
    bzero(readbuff, BUFFER_SIZE);
    sprintf(readbuff, "!@#$!@#$!*@*#*!##(!#*)(@*)");
    n = sendto(sockfd, readbuff, strlen(readbuff), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    fclose(fptr);
    close(sockfd);
    return 0;
}