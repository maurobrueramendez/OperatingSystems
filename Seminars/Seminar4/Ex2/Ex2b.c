#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

int A[100];

void* compute(void* arg) {
    int idx = *(int*)arg;
    free(arg);

    A[idx] = idx * idx;
    return NULL;
}

int main() {
    pthread_t th[100];

    for (int i = 0; i < 100; i++) {
        int* p = malloc(sizeof(int));
        *p = i;
        pthread_create(&th[i], NULL, compute, p);
    }

    for (int i = 0; i < 100; i++)
        pthread_join(th[i], NULL);
}
