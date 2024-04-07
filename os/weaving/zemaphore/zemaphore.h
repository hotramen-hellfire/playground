#include <pthread.h>

typedef struct zemaphore {
    int value;
    pthread_mutex_t chng;
    pthread_cond_t pillow;
} zem_t;

void zem_init(zem_t *, int);
void zem_up(zem_t *);
void zem_down(zem_t *);