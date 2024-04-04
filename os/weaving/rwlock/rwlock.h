#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <iostream>

using namespace std;

struct read_write_lock
{
    pthread_mutex_t chnglock=PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t lassan=PTHREAD_COND_INITIALIZER;
    pthread_cond_t adrak=PTHREAD_COND_INITIALIZER;
    int readercount=0;
    int writerpresent=0;
    int writers_waiting=0;
};

void InitalizeReadWriteLock(struct read_write_lock * rw);
void ReaderLock(struct read_write_lock * rw);
void ReaderUnlock(struct read_write_lock * rw);
void WriterLock(struct read_write_lock * rw);
void WriterUnlock(struct read_write_lock * rw);
