#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char *argv[])
{
    int n;
    scanf("%d", &n);
    int spawner_id = getpid();
    printf("children to be spawned: %d\n", n);
    for (int i = 0; i < n; i++)
    {
        if (getpid() == spawner_id)
        {
            int forker = fork();
        }
    }
    if (getpid() != spawner_id)
    {
        printf("child\n");
    }
    while (getpid() == spawner_id && n--)
    {
        wait(NULL);
        printf("killed a child\n");
    }
    return 0;
}