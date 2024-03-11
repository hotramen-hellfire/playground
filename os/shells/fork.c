#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char *argv[])
{
    printf("----------------------H E L L O----------------------\n");
    int fork_u = fork();
    if (fork_u < 0)
    {
        printf("%d:sorry fork failed :(\n", getpid());
        exit(-1);
    }
    else if (fork_u == 0)
    {
        // children block
        // for (int i = 0; i < argc; i++)
        // {
        //     printf("%s\n", argv[i]);
        // }
        execvp(argv[1], argv + 1);
        // printf("%d: I am the kid!!\n", getpid());
        exit(0);
    }
    else
    {
        // parent block
        int wait_status = wait(NULL);
        printf("----------------------E N D----------------------\n");
        exit(0);
    }
    return 0;
}
