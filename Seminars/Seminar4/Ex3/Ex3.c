#include <pthread.h>
#include <stdlib.h>

int A[10];
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

typedef struct {
    int i, j;
} Move;

void* move(void* arg) {
    Move* m = arg;

    pthread_mutex_lock(&lock);
    if (A[m->i] > 0) {
        A[m->i]--;
        A[m->j]++;
    }
    pthread_mutex_unlock(&lock);

    free(m);
    return NULL;
}
