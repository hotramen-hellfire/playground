#include <stdio.h>
#include <unistd.h>
int main()
{
    int proc = fork();
    if (proc < 0)
    {
        printf("fork failed :(\n");
        exit(-1);
    }
    return 0;
}
