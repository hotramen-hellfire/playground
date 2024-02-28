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
    int soccer = socket(AF_UNIX, SOCK_DGRAM, 0); //this is the socketfd
    struct sockaddr_un serv_addr, client_addr; //this is the socketaddress, it has two variables, sun_path (for socket path) and sun_family=AF_UNIX (for unix ipc)
    bzero((char*)&serv_addr, sizeof(serv_addr));
    serv_addr.sun_family= AF_UNIX; //family
    strcpy(serv_addr.sun_path, SOCK_PATH); //path
        if (soccer < 0)
        perror("ERROR opening socket: ");
    if (bind(soccer, (const struct sockaddr*) &serv_addr, sizeof(serv_addr))<0) //bind to the server
    {
        perror("binding failed!!\nadios amigo :(\n");
        exit(0);
    }
    else
    {
        printf("hola amigo, server binding established!! :)\n");
    }
    char message[20];
    int len =sizeof(client_addr);
    int r=recvfrom(soccer, message, sizeof(message), 0, (struct sockaddr *)&client_addr, &len);//receive and send
    printf("r=%d\nmessage=%s\n",r, message);
    unlink(SOCK_PATH);
    close(soccer);
}