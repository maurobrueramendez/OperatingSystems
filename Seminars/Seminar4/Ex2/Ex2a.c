#include <pthread.h>
#include <stdio.h>

int A[100];

void* compute(void* arg) {
    int idx = *(int*)arg;
    A[idx] = idx * idx;
    return NULL;
}

int main() {
    pthread_t th[100];
    int i;

    for (i = 0; i < 100; i++)
        pthread_create(&th[i], NULL, compute, &i);

    for (i = 0; i < 100; i++)
        pthread_join(th[i], NULL);
}
