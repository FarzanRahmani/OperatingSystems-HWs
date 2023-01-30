#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

typedef struct
{
    int *buffer;
    int n; // buffer size
    int m; 
    int thread_number;
} myarg_t;

sem_t mutex; // mutual exclusion semaphore
sem_t full; // counts the number of full slots
sem_t empty; // counts the number of empty slots
int start = 0; // index of the first element in the buffer
int end = 0; // index of the last element in the buffer

void *produce(void *arg)
{
    myarg_t *args = (myarg_t *)arg;
    int *buffer = args->buffer;
    int n = args->n;
    int m = args->m;
    int thread_number = args->thread_number;
    for (int i = 0; i < m; i++)
    {
        /* produce an item in next_produced */ // generate random number
        int num = rand() % 1000;
        sem_wait(&empty); // decrement the number of empty slots
        sem_wait(&mutex);
        // sleep(1);
        /* add next produced to the buffer */
        buffer[end] = num;
        end = (end + 1) % n;
        printf("Producer Thread %d: Generated number %d\n", thread_number, num);
        sem_post(&mutex);
        sem_post(&full); // increment the number of full slots
    }
    return NULL;
}

void *consume(void *arg)
{
    myarg_t *args = (myarg_t *)arg;
    int *buffer = args->buffer;
    int n = args->n;
    int m = args->m;
    int thread_number = args->thread_number;
    for (int i = 0; i < m; i++)
    {
        sem_wait(&full);  // decrement the number of full slots
        sem_wait(&mutex);
        /* remove an item from buffer to next_consumed */
        int num = buffer[start];
        start = (start + 1) % n;
        char *result = (num % 2 == 0) ? "even" : "odd";
        printf("Consumer Thread %d: I got number %d, and it is %s\n", thread_number, num, result);
        // sleep(1);
        sem_post(&mutex); // signal(mutex)
        sem_post(&empty); // increment the number of empty slots 
        // /* consume the item in next consumed */
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    int n, m;
    printf("Enter the size of buffer: ");
    scanf("%d", &n);
    printf("Enter the number of generated numbers: ");
    scanf("%d", &m);
    int buffer[n]; // shared buffer of size n (n buffers, each can hold one item)
    sem_init(&mutex, 0, 1); // Semaphore mutex initialized to the value 1
    sem_init(&full, 0, 0); // Semaphore full initialized to the value 0
    sem_init(&empty, 0, n); // Semaphore empty initialized to the value n
    // arguments for threads
    myarg_t args1 = {buffer, n, m, 1}; 
    myarg_t args2 = {buffer, n, m, 2};
    myarg_t args3 = {buffer, n, m, 3};
    // producer threads
    pthread_t pt1, pt2, pt3;
    // consumer threads
    pthread_t ct1, ct2, ct3;
    // producer threads call (order of call between producer and consumer is not important)
    pthread_create(&pt1, NULL, produce, (void *)&args1);
    pthread_create(&pt2, NULL, produce, (void *)&args2);
    pthread_create(&pt3, NULL, produce, (void *)&args3);
    // consumer threads call 
    pthread_create(&ct1, NULL, consume, (void *)&args1);
    pthread_create(&ct2, NULL, consume, (void *)&args2);
    pthread_create(&ct3, NULL, consume, (void *)&args3);
    // wait for threads to finish
    pthread_join(pt1, NULL);
    pthread_join(pt2, NULL);
    pthread_join(pt3, NULL);
    pthread_join(ct1, NULL);
    pthread_join(ct2, NULL);
    pthread_join(ct3, NULL);
    // destroy semaphores
    sem_destroy(&mutex);
    sem_destroy(&full);
    sem_destroy(&empty);
    return 0;
}
