#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_RESOURCES 5

int nThreadsAccessing = 0;
sem_t sem;

int getFromDatabase() {
    nThreadsAccessing++;
    printf("Accessing: %d\n", nThreadsAccessing);

    if (nThreadsAccessing > NUM_RESOURCES) _exit(1);

    usleep(rand()%10000);

    nThreadsAccessing--;
    return rand();
}

void* worker(void* arg) {
    sem_wait(&sem);
    getFromDatabase();
    sem_post(&sem);
    return NULL;
}

int main() {
    pthread_t th[20];
    sem_init(&sem, 0, NUM_RESOURCES);

    for (int i = 0; i < 20; i++)
        pthread_create(&th[i], NULL, worker, NULL);

    for (int i = 0; i < 20; i++)
        pthread_join(th[i], NULL);

    sem_destroy(&sem);
}