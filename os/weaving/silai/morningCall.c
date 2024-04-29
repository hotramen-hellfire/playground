#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#define NUM_THREADS 12
pthread_mutex_t printed_mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t condn_mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condn=PTHREAD_COND_INITIALIZER;
struct printerArg{
    int n;
    int* printed;
};
void *printer(void * dummy)
{
    struct printerArg* arg  = (struct printerArg*) dummy;
    // sleep(rand());
    srand(pthread_self());
    int sleepFor=rand()%10;
    printf("%d:  I am sleeping for %ds . . .\n", arg->n, sleepFor);
    sleep(sleepFor);
    while (1)
    {
        pthread_mutex_lock(&printed_mutex);
        int prevDone=1;
        for (int i=0; i<arg->n-1; i++)
        {
            if (arg->printed[i]==0)
            {
                prevDone=0;
                break;
            }
        }
        if (prevDone==1)
        {
            printf("Good Morning, said %d . . .\n", arg->n);
            arg->printed[arg->n-1]=1;
            pthread_cond_broadcast(&condn);
            pthread_mutex_unlock(&printed_mutex);
            break;
        }
        else
        {
            pthread_cond_wait(&condn, &printed_mutex);
        }
        pthread_mutex_unlock(&printed_mutex);
    }
    pthread_exit(NULL);
    return 0x0;
}

int main()
{
    pthread_t threads[NUM_THREADS];
    int printed[NUM_THREADS];
    for (int i=0; i<NUM_THREADS; i++) printed[i]=0;
    for (int i=0; i<NUM_THREADS; i++)
    {
        struct printerArg* arg=(struct printerArg*)malloc(sizeof(struct printerArg));
        arg->n=i+1;
        arg->printed=printed;
        pthread_create(&threads[i], NULL, &printer, (void *)arg);
    }
    for (int i=0; i<NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }
    return 0x0;
}