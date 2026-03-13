// gcc -o ex5 ex5.c -pthread
// ./ex5

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    int *array;
    int start;
    int end;
} ThreadArg;

long sum = 0;
pthread_mutex_t mutex;
pthread_t th[4];
ThreadArg args[4];

/* Assume arr is already loaded with 400 integers */
int arr[400];

void* worker(void* arg) {
    ThreadArg* a = (ThreadArg*) arg;
    long part = 0;

    for(int i = a->start; i < a->end; i++) {
        part += a->array[i];
    }

    pthread_mutex_lock(&mutex);
    sum += part;
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main() {
    pthread_mutex_init(&mutex, NULL);

    for(int i = 0; i < 4; i++) {
        args[i].array = arr;
        args[i].start = i * 100;
        args[i].end = (i + 1) * 100;
        pthread_create(&th[i], NULL, worker, &args[i]);
    }

    for(int i = 0; i < 4; i++) {
        pthread_join(th[i], NULL);
    }

    printf("Total sum: %ld\n", sum);

    pthread_mutex_destroy(&mutex);
    return 0;
}