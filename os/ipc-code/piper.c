#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

char *ptr = "Hello client!!\0";
char *pathname = "/tmp/aloha";

int main()
{
    int fd;
    if (mkfifo(pathname, 0666) < 0)
    {
        printf("named pipe exists!!\n");
        // return -1;
    }
    printf("Sending:- Hello client!!, length=%d\n", strlen(ptr));
    fd = open(pathname, O_WRONLY);
    write(fd, ptr, strlen(ptr) + 1);
    close(fd);
    return 0;
}