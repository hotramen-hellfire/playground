#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <wait.h>
#include <pthread.h>
#include <assert.h>
int item_to_produce, curr_buf_size;
int total_items, max_buf_size, num_workers, num_masters;
pthread_mutex_t buff_lock;
pthread_cond_t master_sleeps;
pthread_cond_t worker_sleeps;
int masters_exit=0;
int *buffer;

void print_produced(int num, int master) {

  printf("Produced %d by master %d\n", num, master);
}

void print_produced1(int num, int master, int idx) {

  // printf("Produced %d by master %d at idx %d\n", num, master, idx);
}

void print_consumed(int num, int worker) {

  printf("Consumed %d by worker %d\n", num, worker);
  
}

void print_consumed1(int num, int worker, int idx) {

  // printf("Consumed %d by worker %d at idx %d\n", num, worker, idx);
}

//produce items and place in buffer
//modify code below to synchronize correctly
void *generate_requests_loop(void *data)
{
  int thread_id = *((int *)data);

  while(1)
    {
      pthread_mutex_lock(&buff_lock);
      if(item_to_produce >= total_items) {
        pthread_mutex_unlock(&buff_lock);
        break;
      }
      while (curr_buf_size==max_buf_size && item_to_produce<total_items) pthread_cond_wait(&master_sleeps, &buff_lock);
      buffer[curr_buf_size++] = item_to_produce;
      print_produced1(item_to_produce, thread_id, curr_buf_size-1);
      print_produced(item_to_produce, thread_id);
      item_to_produce++;
      pthread_cond_broadcast(&worker_sleeps);//the master might be sleeping/ one of the masters pick up
      pthread_mutex_unlock(&buff_lock);
    }
  return 0;
}

void* consumers(void* data)
{
  int thread_id = *((int *)data);
  // return 0x0;
  while(1)
  {
    pthread_mutex_lock(&buff_lock);
    while (curr_buf_size==0 && masters_exit==0) pthread_cond_wait(&worker_sleeps, &buff_lock);
    if (curr_buf_size==0 && masters_exit==1)
    {
        pthread_mutex_unlock(&buff_lock);
        break; 
    }
    int read_value=buffer[--curr_buf_size];// read and update the curr_buf_size
    print_consumed1(read_value, thread_id, curr_buf_size);
    print_consumed(read_value, thread_id);
    pthread_cond_broadcast(&master_sleeps);//the master might be sleeping/ one of the masters pick up
    pthread_mutex_unlock(&buff_lock);
  }
  return 0;
}

//write function to be run by worker threads
//ensure that the workers call the function print_consumed when they consume an item

int main(int argc, char *argv[])
{
  int *master_thread_id;
  pthread_t *master_thread;
  int *worker_thread_id;
  pthread_t *worker_thread;
  item_to_produce = 0;
  curr_buf_size = 0;
  
  int i;
  
   if (argc < 5) {
    printf("./master-worker #total_items #max_buf_size #num_workers #masters e.g. ./exe 10000 1000 4 3\n");
    exit(1);
  }
  else {
    num_masters = atoi(argv[4]);
    num_workers = atoi(argv[3]);
    total_items = atoi(argv[1]);
    max_buf_size = atoi(argv[2]);
  }
    

   buffer = (int *)malloc (sizeof(int) * max_buf_size);

   //create master producer threads
   master_thread_id = (int *)malloc(sizeof(int) * num_masters);
   master_thread = (pthread_t *)malloc(sizeof(pthread_t) * num_masters);
  for (i = 0; i < num_masters; i++)
    master_thread_id[i] = i;

  for (i = 0; i < num_masters; i++)
    pthread_create(&master_thread[i], NULL, generate_requests_loop, (void *)&master_thread_id[i]);
  
  //create worker consumer threads
   worker_thread_id = (int *)malloc(sizeof(int) * num_workers);
   worker_thread = (pthread_t *)malloc(sizeof(pthread_t) * num_workers);
  for (i = 0; i < num_workers; i++)
    worker_thread_id[i] = i;

  for (i = 0; i < num_workers; i++)
    pthread_create(&worker_thread[i], NULL, consumers, (void *)&worker_thread_id[i]);

  
  //wait for all threads to complete
  for (i = 0; i < num_masters; i++)
    {
      // pthread_cond_broadcast(&master_sleeps);//the master might be sleeping/ one of the masters pick up 
      pthread_join(master_thread[i], NULL);
      printf("master %d joined\n", i);
    }
  
  pthread_mutex_lock(&buff_lock);
  pthread_cond_broadcast(&worker_sleeps);//the master might be sleeping/ one of the masters pick up
  masters_exit=1;
  pthread_mutex_unlock(&buff_lock);
  
  // printf("hwew\n");
  //wait for all threads to complete
  for (i = 0; i < num_workers; i++)
    {
      pthread_join(worker_thread[i], NULL);
      printf("worker %d joined\n", i);
    }
  
  /*----Deallocating Buffers---------------------*/
  free(buffer);
  free(master_thread_id);
  free(master_thread);
  
  return 0;
}
