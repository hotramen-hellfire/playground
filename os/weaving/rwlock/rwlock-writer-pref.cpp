#include "rwlock.h"


void InitalizeReadWriteLock(struct read_write_lock * rw)
{
  //	Write the code for initializing your read-write lock.
  rw->chnglock=PTHREAD_MUTEX_INITIALIZER;
  rw->lassan=PTHREAD_COND_INITIALIZER;
  rw->adrak=PTHREAD_COND_INITIALIZER;
  rw->readercount=0;
  rw->writerpresent=0;
  rw->writers_waiting=0;
}

void ReaderLock(struct read_write_lock * rw)
{
  //	Write the code for aquiring read-write lock by the reader.
  pthread_mutex_lock(&rw->chnglock);
  while (rw->writerpresent || rw->writers_waiting) pthread_cond_wait(&rw->lassan, &rw->chnglock);
  rw->readercount++;
  pthread_mutex_unlock(&rw->chnglock);
}

void ReaderUnlock(struct read_write_lock * rw)
{
  //	Write the code for releasing read-write lock by the reader.
  pthread_mutex_lock(&rw->chnglock);
  rw->readercount--;
  pthread_cond_signal(&rw->adrak);//writer might be sleeping on adrak
  pthread_mutex_unlock(&rw->chnglock);
}

void WriterLock(struct read_write_lock * rw)
{
  //	Write the code for aquiring read-write lock by the writer.
  pthread_mutex_lock(&rw->chnglock);
  rw->writers_waiting++;
  while (rw->readercount>0 || rw->writerpresent==1) pthread_cond_wait(&rw->adrak, &rw->chnglock);
  rw->writers_waiting--;
  rw->writerpresent=1;
  pthread_mutex_unlock(&rw->chnglock);
}

void WriterUnlock(struct read_write_lock * rw)
{
  //	Write the code for releasing read-write lock by the writer.
  pthread_mutex_lock(&rw->chnglock);
  rw->writerpresent=0;
  if (rw->writers_waiting>0) 
  pthread_cond_broadcast(&rw->adrak);//writer might be sleeping on adrak
  pthread_cond_broadcast(&rw->lassan);//signal the next reader waiting on lassan
  pthread_mutex_unlock(&rw->chnglock);
}
