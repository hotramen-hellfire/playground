#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <wait.h>
#include "zemaphore.h"

void zem_init(zem_t *s, int value) {
  s->chng=PTHREAD_MUTEX_INITIALIZER;
  s->pillow=PTHREAD_COND_INITIALIZER;
  s->value=value;
}

void zem_down(zem_t *s) {
    pthread_mutex_lock(&s->chng);
    s->value--;
    if (s->value<0)
    {
        pthread_cond_wait(&s->pillow, &s->chng);//sleep on pillow
    }
    pthread_mutex_unlock(&s->chng);
}

void zem_up(zem_t *s) {
    pthread_mutex_lock(&s->chng);
    s->value++;
    pthread_cond_signal(&s->pillow);
    pthread_mutex_unlock(&s->chng);
}
