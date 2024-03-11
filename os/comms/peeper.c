#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

char *ptr = "hola";
char *pathname = "/tmp/aloha";

int main()
{
    int fd = open(pathname, O_RDONLY);
    char messagerecv[45];
    read(fd, messagerecv, sizeof(messagerecv));
    // printf("%d\n", sizeof(messagerecv));
    printf(messagerecv);
    // write(fd, message, strlen(message) + 1);
    close(fd);
    return 0;
}