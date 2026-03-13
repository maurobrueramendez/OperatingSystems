// gcc -o ex4 ex4.c -pthread
// ./ex4

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>

typedef struct {
    int id;
} ThreadArg;

pthread_mutex_t mutex;
pthread_t th[4];
ThreadArg args[4];

void* worker(void* arg) {
    ThreadArg* a = (ThreadArg *) arg;
    pthread_mutex_lock(&mutex);
    printf("Hello from thread %d\n", a->id);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main() {
    pthread_mutex_init(&mutex, NULL);
    for(int i = 0; i < 4; i++) {
        args[i].id = i + 1;
        pthread_create(&th[i], NULL, worker, &args[i]);
    }
    for(int i = 0; i < 4; i++) {
        pthread_join(th[i], NULL);
    }
    pthread_mutex_destroy(&mutex);
    return 0;
}