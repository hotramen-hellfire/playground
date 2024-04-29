#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
struct printer_arg
{
    int num;
    int total;
    int* next;
    pthread_mutex_t *lock;
    pthread_cond_t *condvar;
};
void *printer(void *_arg)
{
    struct printer_arg *arg = (struct printer_arg *)_arg;
    int num = arg->num;
    int* next=arg->next;
    pthread_mutex_lock(arg->lock);
    while (1)
    {
        while (*next != num)
            pthread_cond_wait(arg->condvar, arg->lock);
        printf("%d\n", *next);
        sleep(1);
        *next = *next + 1;
        *next=(*next)%(arg->total);
        pthread_cond_broadcast(arg->condvar);
    }
    pthread_mutex_unlock(arg->lock);
    pthread_exit(NULL);
}
int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("give an integer argument!!\n");
        return -1;
    }
    int next=0;
    int n = atoi(argv[1]);
    pthread_t threadids[n];
    struct printer_arg args[n];
    pthread_mutex_t mylock = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t mycond = PTHREAD_COND_INITIALIZER;
    for (int i = 0; i < n; i++)
    {
        args[i].num = i;
        args[i].condvar = &mycond;
        args[i].lock = &mylock;
        args[i].total = n;
        args[i].next=&next;
        pthread_create(&threadids[i], NULL, &printer, (void *)&args[i]);
    }
    for (int i = 0; i < n; i++)
        pthread_join(threadids[i], NULL);
    return 0;
}