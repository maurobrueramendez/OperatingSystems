// gcc -o ex3 ex3.c -pthread
// ./ex3

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

long counter = 0;
pthread_mutex_t mutex;
pthread_t th[4];

void* worker(void* arg) {
    for(int i = 0; i < 1000; i++) {
        pthread_mutex_lock(&mutex);
        counter++;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    pthread_mutex_init(&mutex, NULL);
    for(int i = 0; i < 4; i++) {
        pthread_create(&th[i], NULL, worker, NULL);
    }
    for(int i = 0; i < 4; i++) {
        pthread_join(th[i], NULL);
    }
    printf("Counter = %ld\n", counter);
    return 0;
}