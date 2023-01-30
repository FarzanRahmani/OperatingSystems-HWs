#include "stdlib.h"
#include "stdio.h"
#include "unistd.h"
#include "string.h"
#include "pthread.h" // gcc -pthread -o term term.c
#include <assert.h>
#include <semaphore.h>

// using barrier to synchronize threads in matrix multiplication
// using semaphores to synchronize threads in matrix multiplication

sem_t barrier1;
sem_t barrier2;
sem_t mutex;
int count;
int num_of_threads;

// 2D matrix multiplication (A^5)
// A = 2x2
int A[2][2] = {{1, 2}, {3, 4}};
int A2[2][2] = {{0, 0}, {0, 0}};
int A3[2][2] = {{0, 0}, {0, 0}};
int A4[2][2] = {{0, 0}, {0, 0}};
int A5[2][2] = {{0, 0}, {0, 0}};

void *multiply_matrix(void *arg) {
    int *row_col = (int *)arg;
    int row = row_col[0];
    int col = row_col[1];
    int i;

    // compute A^2
    for (i = 0; i < 2; i++) {
        A2[row][col] += A[row][i] * A[i][col];
    }
    
    // Two-Phase Barrier
    sem_wait(&mutex);
    count++;
    if (count == num_of_threads)
    {
        sem_wait(&barrier2);
        sem_post(&barrier1);
    }
    sem_post(&mutex);

    sem_wait(&barrier1);
    sem_post(&barrier1);
    // barrier1 = 1 for all processes, barrier2 = 0 for all processes
    sem_wait(&mutex);
    count--;
    if (count == 0)
    {
        sem_wait(&barrier1);
        sem_post(&barrier2);
    }
    sem_post(&mutex);

    sem_wait(&barrier2);
    sem_post(&barrier2);
    // barrier2 = 1 for all processes, barrier1 = 0 for all processes

    // barrier
    // sem_wait(&mutex);
    // count++;
    // if (count == num_of_threads) { // last thread to reach barrier
    //     count = 0;
    //     sem_post(&mutex);
    //     for (i = 0; i < num_of_threads - 1; i++) { // release all threads (num_of_threads - 1)
    //         sem_post(&barrier);
    //     }
    // } else {
    //     sem_post(&mutex);
    //     sem_wait(&barrier); // wait for all threads to finish (3 threads get stuck here)
    // }

    // compute A^3
    for (i = 0; i < 2; i++) {
        A3[row][col] += A2[row][i] * A[i][col];
    }
    // Two-Phase Barrier
    sem_wait(&mutex);
    count++;
    if (count == num_of_threads)
    {
        sem_wait(&barrier2);
        sem_post(&barrier1);
    }
    sem_post(&mutex);

    sem_wait(&barrier1);
    sem_post(&barrier1);
    // barrier1 = 1 for all processes, barrier2 = 0 for all processes
    sem_wait(&mutex);
    count--;
    if (count == 0)
    {
        sem_wait(&barrier1);
        sem_post(&barrier2);
    }
    sem_post(&mutex);

    sem_wait(&barrier2);
    sem_post(&barrier2);
    // barrier2 = 1 for all processes, barrier1 = 0 for all processes

    // barrier
    // sem_wait(&mutex);
    // count++;
    // sem_post(&mutex);
    // if (count == num_of_threads)
    // {
    //     count = 0;
    //     sem_post(&barrier);
    // }
    // sem_wait(&barrier);
    // sem_post(&barrier);  // turnstile: a wait and a post occurs in  
    
    // compute A^4
    for (i = 0; i < 2; i++) {
        A4[row][col] += A3[row][i] * A[i][col];
    }
    // Two-Phase Barrier
    sem_wait(&mutex);
    count++;
    if (count == num_of_threads)
    {
        sem_wait(&barrier2);
        sem_post(&barrier1);
    }
    sem_post(&mutex);

    sem_wait(&barrier1);
    sem_post(&barrier1);
    // barrier1 = 1 for all processes, barrier2 = 0 for all processes
    sem_wait(&mutex);
    count--;
    if (count == 0)
    {
        sem_wait(&barrier1);
        sem_post(&barrier2);
    }
    sem_post(&mutex);

    sem_wait(&barrier2);
    sem_post(&barrier2);
    // barrier2 = 1 for all processes, barrier1 = 0 for all processes

    // compute A^5
    for (i = 0; i < 2; i++) {
        A5[row][col] += A4[row][i] * A[i][col];
    }
    
}

int main(int argc, char *argv[]) {
    // print A
    printf("compute A^5 using 4 threads with barrier using semaphore\n");
    printf("A = \n");
    for (int i = 0; i < 2; i++) {
        printf("[%d %d]\n", A[i][0], A[i][1]);
    }
    
    // initialize semaphores
    sem_init(&barrier1, 0, 0); // two-phase barrier
    sem_init(&barrier2, 0, 1);
    sem_init(&mutex, 0, 1);
    num_of_threads = 4;
    count = 0;
    
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

    // print A5
    printf("A5 = \n");
    for (i = 0; i < 2; i++) {
        printf("[%d %d]\n", A5[i][0], A5[i][1]);
    }

    return 0;
}