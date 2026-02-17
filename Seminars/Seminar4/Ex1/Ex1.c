#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

#define N 100

void* worker(void* arg) {
    sleep(1);
    return NULL;
}

int main() {
    pthread_t th[N];

    for (int i = 0; i < N; i++)
        pthread_create(&th[i], NULL, worker, NULL);

    for (int i = 0; i < N; i++)
        pthread_join(th[i], NULL);

    printf("All threads finished\n");
    return 0;
}