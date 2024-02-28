#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>

#define SOCK_PATH "santa_monica"
#define BUFFER_SIZE 48
char *EOF_path = "/tmp/EOF_pipe";

int main()
{
    int sockfd;
    char buffer[BUFFER_SIZE];
    // mkfifo(EOF_path, 0666);
    char buff[10];
    // int EOF_pipe = open(EOF_path, O_RDONLY);
    struct sockaddr_un server_addr, client_addr;
    int n, n0;
    sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sockfd < 0)
        perror("ERROR opening socket: ");
    bzero((char *)&server_addr, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    
    strcpy(server_addr.sun_path, SOCK_PATH);
    
    printf("STARTED SERVER @ %s\n", SOCK_PATH);
    if (bind(sockfd, (struct sockaddr *)&server_addr,
             sizeof(server_addr)) < 0)
        perror("ERROR on binding: ");
    while (1)
    {
        bzero(buffer, BUFFER_SIZE);
        int len = sizeof(client_addr);
       
        n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &len);
        // read(EOF_pipe, buff, 10);
        // sscanf(buff, "%d", &n0);
        // if (n0 == 1)
        // {
        //     printf("\nCONCERT ENDED!!\n");
        //     break;
        // }
        // if (n0 < 0)
        // {
        //     printf("\nBROADCASTING ERROR!! :(\n");
        //     break;
        // }
        if (strcmp(buffer, "!@#$!@#$!*@*#*!##(!#*)(@*)") == 0)
        {
            printf("\nCONCERT ENDED!!\n");
            fflush(stdout);
            break;
        }
        if (n < 0)
        {
            perror("\nERROR IN BRODCAST :(\n");
        }
        printf(buffer);
        fflush(stdout);
    }

    unlink(SOCK_PATH);
    // close(EOF_pipe);
    return 0;
}