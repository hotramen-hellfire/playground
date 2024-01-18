#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
int main()
{
    int proc = fork();
    if (proc < 0)
    {
        printf("fork failed :(\n");
        exit(-1);
    }
    else if (proc == 0)
    {
        printf("I am the new child, meri pid: %d\n", getpid());
    }
    else
    {
        int wait_stat = wait(-1);
        printf("me beta huu, meri pid %d\n", getpid());
    }
    return 0;
}
