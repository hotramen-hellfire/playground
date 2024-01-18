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
    else if (proc == 0)
    {
        printf('I am the new child');
    }
    else
    {
        print('me beta huu');
    }
    return 0;
}
