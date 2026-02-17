#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

int A[100];

void* compute(void* arg) {
    int idx = *(int*)arg;
    free(arg);              // free pointer
    A[idx] = idx * idx;     // A[i] = i * i
    return NULL;
}

int main() {
    pthread_t th[100];

    for (int i = 0; i < 100; i++) {
        int* p = malloc(sizeof(int));       // initialise pointer
        *p = i;                             // point to copy of index value
        pthread_create(&th[i], NULL, compute, p);       // pass pointer to a copy of index value
    }

    for (int i = 0; i < 100; i++)
        pthread_join(th[i], NULL);
}