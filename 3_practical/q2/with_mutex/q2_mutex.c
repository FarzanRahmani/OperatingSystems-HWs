#include "stdlib.h"
#include "stdio.h"
#include "unistd.h"
#include "string.h"
#include "pthread.h" // gcc -pthread -o term term.c
#include <assert.h>
#include <semaphore.h>


// 2D matrix multiplication (A^5)
// A = 2x2
int A[2][2] = {{1, 2}, {3, 4}};
int A2[2][2] = {{0, 0}, {0, 0}};
int A3[2][2] = {{0, 0}, {0, 0}};

typedef union
{
    int count;
} brrier_t;

void initialize_barrier(brrier_t *b_t, int count)
{
    b_t->count = count;
}

int barriernum = 4;
pthread_mutex_t lock;
pthread_mutex_t barrier;
brrier_t b;

void barrierwait(brrier_t *bt)
{
    pthread_mutex_lock(&lock);
    bt->count = bt->count - 1;
    pthread_mutex_unlock(&lock);
    if (bt->count == 0)
    {
        pthread_mutex_unlock(&barrier);
    }
    else // (bt->count > 0)
    {
        pthread_mutex_lock(&barrier);
        pthread_mutex_unlock(&barrier);
    }
    // after finisheing status bit -> -1 (for to do something in a serial region before entering another parallel section of code.)
}

void *multiply_matrix(void *arg) {
    int *row_col = (int *)arg;
    int row = row_col[0];
    int col = row_col[1];
    int i;

    // compute A^2
    for (i = 0; i < 2; i++) {
        A2[row][col] += A[row][i] * A[i][col];
    }
    
    barrierwait(&b);

    // compute A^3
    for (i = 0; i < 2; i++) {
        A3[row][col] += A2[row][i] * A[i][col];
    }
    
}

int main()
{
    printf("compute A^3 using 4 threads with barrier using mutex only\n");
    printf("A = \n");
    for (int i = 0; i < 2; i++) {
        printf("[%d %d]\n", A[i][0], A[i][1]);
    }

    initialize_barrier(&b, 4);
    pthread_mutex_init(&lock, NULL);
    pthread_mutex_init(&barrier, NULL);
    pthread_mutex_lock(&barrier);

    // threads ids
    pthread_t worker_thread_ids[4];
    int i;
    for (i = 0; i < 4; i++) {
        int *row_col = malloc(sizeof(int) * 2);
        row_col[0] = i / 2; // 0, 0, 1, 1
        row_col[1] = i % 2; // 0, 1, 0, 1
        pthread_create(&worker_thread_ids[i], NULL, multiply_matrix, (void *)row_col);
    }

    for (i = 0; i < 4; i++) {
        pthread_join(worker_thread_ids[i], NULL);
    }

    // print A3
    printf("A3 = \n");
    for (i = 0; i < 2; i++) {
        printf("[%d %d]\n", A3[i][0], A3[i][1]);
    }

    return 0;
}