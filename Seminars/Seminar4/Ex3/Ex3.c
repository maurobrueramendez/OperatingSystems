#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int A[10];
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

typedef struct {
    int i, j;
} Move;

// particle move
// if A[i] > 0, move ONE particle from i to j (A[i]--, A[j]++) 
void* move(void* arg) {
    Move* m = (Move*)arg;

    pthread_mutex_lock(&lock);
    if (A[m->i] > 0) {
        A[m->i]--;
        A[m->j]++;
    }
    pthread_mutex_unlock(&lock);

    free(m);
    return NULL;
}

// helpers
static void init_particles(void) {
    for (int k = 0; k < 10; k++) A[k] = rand() % 3; // 0..2
}

static void print_particles(const char* label) {
    printf("%s ", label);
    for (int k = 0; k < 10; k++) printf("%d ", A[k]);
    printf("\n");
}

// Part (a) 
// 0->1, 2->3, 4->5, 6->7, 8->9
static void part_a_even_to_odd(void) {
    pthread_t th[5];

    for (int t = 0; t < 5; t++) {
        Move* m = (Move*)malloc(sizeof(Move));
        m->i = 2 * t;
        m->j = 2 * t + 1;
        pthread_create(&th[t], NULL, move, m);
    }

    for (int t = 0; t < 5; t++) {
        pthread_join(th[t], NULL);
    }
}

// Part (b) 
// 10 random moves, choose random i and j for each thread
static void part_b_random_moves(void) {
    pthread_t th[10];

    for (int t = 0; t < 10; t++) {
        Move* m = (Move*)malloc(sizeof(Move));
        m->i = rand() % 10;
        m->j = rand() % 10;
        pthread_create(&th[t], NULL, move, m);
    }

    for (int t = 0; t < 10; t++) {
        pthread_join(th[t], NULL);
    }
}

// Part (c) 
// wind blows from low index to high index:
// particles from cell k move to cell k+1 (for k=0..8), cell 9 cannot move

// avoids order effects from in-place updates
typedef struct {
    int k;           // source cell index 0..8
    int* oldA;       // snapshot array
    int* B;          // target array
} WindArg;

static void* wind_worker(void* arg) {
    WindArg* w = (WindArg*)arg;
    int k = w->k;

    // all particles in oldA[k] move to B[k+1]
    w->B[k + 1] = w->oldA[k];

    free(w);
    return NULL;
}

static void part_c_wind_shift(void) {
    int oldA[10];
    int B[10];

    // snapshot current state 
    for (int k = 0; k < 10; k++) oldA[k] = A[k];

    // start B with zeros
    for (int k = 0; k < 10; k++) B[k] = 0;

    // cell 0 becomes 0 because everything moves right
    B[0] = 0;

    // spawn threads for k = 0..8, each sets B[k+1] = oldA[k]
    pthread_t th[9];
    for (int k = 0; k <= 8; k++) {
        WindArg* w = (WindArg*)malloc(sizeof(WindArg));
        w->k = k;
        w->oldA = oldA;
        w->B = B;
        pthread_create(&th[k], NULL, wind_worker, w);
    }

    for (int k = 0; k <= 8; k++) {
        pthread_join(th[k], NULL);
    }

    // cell 9 can't move, so it keeps its old particles + receives from cell 8
    // after the threads, B[9] already has oldA[8]; now add oldA[9]
    B[9] += oldA[9];

    // copy B back into A
    for (int k = 0; k < 10; k++) A[k] = B[k];
}

int main(void) {
    srand((unsigned)time(NULL));

    // Part (a) 
    init_particles();
    printf("PART (a): even -> next odd\n");
    print_particles("Before:");
    part_a_even_to_odd();
    print_particles("After :");
    printf("\n");

    // Part (b) 
    init_particles();
    printf("PART (b): 10 random moves\n");
    print_particles("Before:");
    part_b_random_moves();
    print_particles("After :");
    printf("\n");

    // Part (c) 
    init_particles();
    printf("PART (c): wind shift (simultaneous step)\n");
    print_particles("Before:");
    part_c_wind_shift();
    print_particles("After :");
    printf("\n");

    return 0;
}