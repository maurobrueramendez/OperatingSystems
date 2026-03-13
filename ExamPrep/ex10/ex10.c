// gcc -o ex10 ex10.c -pthread
// ./ex10

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>  
#include <pthread.h>

typedef struct {
    int *arr;
    int start;
    int end;
} ThreadArg;

long total_even = 0;
int array[400];
pthread_mutex_t mutex;
pthread_t th[4];
ThreadArg args[4];

void* worker(void* arg) {
    ThreadArg* a = (ThreadArg *) arg;
    long temp = 0;
    for(int i = a->start; i < a->end; i++) {
        if(a->arr[i] % 2 == 0) {
            temp++;
        }
    }

    pthread_mutex_lock(&mutex);
    total_even += temp;
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main() {
    pthread_mutex_init(&mutex, NULL);
    for(int i = 0; i < 4; i++) {
        args[i].arr = array;
        args[i].start = i * 100;
        args[i].end = (i+1) * 100;
        pthread_create(&th[i], NULL, worker, &args[i]);
    }

    for(int i = 0; i < 4; i++) {
        pthread_join(th[i], NULL);
    }

    printf("Total even numbers: %ld\n", total_even);

    pthread_mutex_destroy(&mutex);
    return 0;
}