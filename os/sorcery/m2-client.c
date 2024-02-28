#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <fcntl.h>
#define SOCK_PATH "onion"
int main()
{
    int soccer = socket(AF_UNIX, SOCK_DGRAM, 0);
    struct sockaddr_un serv_addr;
    bzero((char*)&serv_addr, sizeof(serv_addr));
    serv_addr.sun_family= AF_UNIX;
    strcpy(serv_addr.sun_path, SOCK_PATH);
    char message[20]="good!!";
    int r=sendto(soccer, message, sizeof(message), 0, (const struct sockaddr*)&serv_addr, sizeof(serv_addr));
    close(soccer);
}
