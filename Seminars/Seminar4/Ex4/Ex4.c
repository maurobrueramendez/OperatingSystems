#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "semaphore.h"   

#define NUM_RESOURCES 5
#define NTHREADS 20

int nThreadsAccessing = 0;
Semaphore sem;           

int getFromDatabase(void) {
    nThreadsAccessing++;
    printf("Accessing: %d\n", nThreadsAccessing);

    if (nThreadsAccessing > NUM_RESOURCES) _exit(1);

    usleep(rand() % 10000);

    nThreadsAccessing--;
    return rand();
}

void* worker(void* arg) {
    (void)arg;

    sem_wait(&sem);
    getFromDatabase();
    sem_post(&sem);

    return NULL;
}

int main(void) {
    pthread_t th[NTHREADS];

    sem_init(&sem, NUM_RESOURCES);

    for (int i = 0; i < NTHREADS; i++)
        pthread_create(&th[i], NULL, worker, NULL);

    for (int i = 0; i < NTHREADS; i++)
        pthread_join(th[i], NULL);

    sem_destroy(&sem);
    return 0;
}