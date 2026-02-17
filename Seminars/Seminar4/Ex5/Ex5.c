#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_RES 5

int available = MAX_RES;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void request_resources(int rank) {
    pthread_mutex_lock(&m);

    while (available < rank)
        pthread_cond_wait(&cond, &m);

    available -= rank;
    printf("Acquire rank %d -> left %d\n", rank, available);

    pthread_mutex_unlock(&m);
}

void release_resources(int rank) {
    pthread_mutex_lock(&m);

    available += rank;
    printf("Release rank %d -> left %d\n", rank, available);

    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&m);
}

void* worker(void* arg) {
    int rank = rand()%3 + 1;

    request_resources(rank);
    usleep(rand()%10000);
    release_resources(rank);

    return NULL;
}
