#include <stdio.h>
#include <pthread.h> 
#include <stdlib.h>
#define NUM_THREADS 32
#define COUNTER 1000000
int counter=COUNTER;
pthread_mutex_t mutex;
int _inc_counter(void)
{
	// int ncounter=COUNTER;
	while (1) 
	{
		pthread_mutex_lock(&mutex);
		if (counter<=0) 
		{
			pthread_mutex_unlock(&mutex);
			// printf("ncounter: %d\n", ncounter);
			break;
		}
		counter--;
		printf("%lld: %d\n", pthread_self(), counter);
		// ncounter--;
		pthread_mutex_unlock(&mutex);
	}
	pthread_exit(NULL);
}

int main()
{
	pthread_t ptids[NUM_THREADS];
	for (int i=0; i<NUM_THREADS; i++)
	{
		//create the threads
		int res=pthread_create(&ptids[i], NULL, (void *)_inc_counter, NULL);
		if (res)
		{
			perror("create error: ");
			exit(-1);
		}
	}
	for (int i=0; i<NUM_THREADS; i++)
	{
		//create the threads
		pthread_join(ptids[i], NULL);
	}

	printf("counter: %d\n", counter);	
	pthread_exit(NULL);
	return 0;
}
