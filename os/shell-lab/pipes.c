#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{
    int pipefd[2];
    char buf;
    pid_t cpid;

    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    cpid = fork();
    if (cpid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (cpid == 0)
    {
        // child code
        close(pipefd[1]);
        while (read(pipefd[0], &buf, 1) > 0)
        {
            // fprintf(stdout, "%s", buf);
            write(1, &buf, 1);
        };
        printf("this is the end, of the child!!\n");
        close(pipefd[0]);
    }
    else
    {
        // parent code
        close(pipefd[0]);
        write(pipefd[1], "this is the message parent sent, !!\n", strlen("this is the message parent sent, !!\n"));
        printf("this is the end of the parent!!, gnwngn. bye1!1!\n");
        close(pipefd[1]);
        wait(NULL);
    }
    return 1;
}