// gcc -o ex14 ex14.c -pthread
// ./ex14

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

typedef struct {
    int id;
} ThreadArg;

int flag = 1;
pthread_mutex_t mutex;
pthread_t th[10];
ThreadArg args[10];

void* worker(void* arg) {
    ThreadArg* a = (ThreadArg *) arg;
    pthread_mutex_lock(&mutex);

    if(flag == 1) {
        flag = 0;
        printf("Thread id: %d\n", a->id);
    }

    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main() {
    pthread_mutex_init(&mutex, NULL);

    for(int i = 0; i < 10; i++) {
        args[i].id = i + 1;
        pthread_create(&th[i], NULL, worker, &args[i]);
    }

    for(int i = 0; i < 10; i++) {
        pthread_join(th[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    return 0;
}
