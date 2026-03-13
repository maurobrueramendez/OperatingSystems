// gcc -o ex12 ex12.c -pthread
// ./ex12

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>

#define N 10

typedef struct {
    int i;
    int j;
} ThreadArg;

int A[N] = {0, 0, 2, 1, 2, 1, 1, 0, 1, 2};
pthread_mutex_t lock;
pthread_cond_t cond;
int acc[N] = {0};

void* worker(void* arg) {
    ThreadArg* a = (ThreadArg *) arg;
    pthread_mutex_lock(&lock);
    while(acc[a->i] || acc[a->j]) {
        pthread_cond_wait(&cond, &lock);
    }

    acc[a->i] = 1;
    acc[a->j] = 1;

    if(A[a->i] > 0) {
        A[a->i]--;
        A[a->j]++;
    }

    acc[a->i] = 0;
    acc[a->j] = 0;

    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&lock);

    return NULL;
}

int main() {
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&cond, NULL);

    // thread creation and wait

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);

    return 0;
}