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
        int wait_stat = wait(NULL);
        printf("mAI ISKA bAAP huu, meri pid %d hai, mene apne bete pid: %d ko maar dala hai\n", getpid(), wait_stat);
    }
    return 0;
}
