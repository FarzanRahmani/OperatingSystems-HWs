#include "stdlib.h"
#include "stdio.h"
#include "unistd.h"
#include "string.h"
#include "pthread.h" // gcc -pthread -o term term.c
#include <assert.h>
#include <semaphore.h>

sem_t self[5]; // condition self [5];
enum
{
    THINKING,
    HUNGRY,
    EATING
} state[5];
int philosopher_nums[5] = { 0, 1, 2, 3, 4 };


sem_t mutex; // for mutual exclusion between prints (don't get mixed up)

void test(int i)
{
    if ((state[(i + 4) % 5] != EATING) &&
        (state[i] == HUNGRY) &&
        (state[(i + 1) % 5] != EATING))
    {
        state[i] = EATING;
        sem_post(&self[i]); // self[i].signal () ;
        printf("Philosopher %d picks chopstick %d and %d up\n", i + 1, ((i + 4) % 5) + 1, i + 1);
        printf("Philosopher %d is Eating\n", i + 1);
    }
}

void pickup(int i)
{
    sem_wait(&mutex);
    state[i] = HUNGRY;
    printf("Philosopher %d is Hungry\n", i + 1);
    test(i); // test if neighbors are eating or not (if not, eat) (for philosopher itself)
    sem_post(&mutex);
    // if (state[i] != EATING)
    // {
        sem_wait(&self[i]); // wait for chopsticks to become free // if unable to eat wait to be signalled
    // }
}

void putdown(int i)
{
    sem_wait(&mutex);
    state[i] = THINKING;
    printf("Philosopher %d putting chopstick %d and %d down\n", i + 1, ((i + 4) % 5) + 1, i + 1);
    printf("Philosopher %d is thinking\n", i + 1);
    // test left and right neighbors
    test((i + 4) % 5); // test left neighbor
    test((i + 1) % 5); // test right neighbor
    sem_post(&mutex);
}

void *philosopher(void *arg)
{
    // int i = (int) arg;
    int *i = (int *)arg;
    while (1)
    {
        pickup(*i); // become hungry

        /** EAT **/
        sleep(1);

        putdown(*i);
        
        /** THINK **/
        sleep(0.5);
    }
}

int main(int argc, char *argv[])
{
    // dininig philosophers problem using montior
    // No deadlock, but starvation is possible
    // 5 philosophers, 5 chopsticks
    // condition self[5];
    for (int i = 0; i < 5; i++)
    {
        sem_init(&self[i], 0, 0);
    }

    // initialization_code()
    for (int i = 0; i < 5; i++)
    {
        state[i] = THINKING;
    }

    sem_init(&mutex, 0, 1);

    // threads ids
    pthread_t tid[5];
    for (int i = 0; i < 5; i++)
    {
        // create philosopher processes
        // pthread_create(&tid, NULL, philosopher, (void *) &i);
        pthread_create(&tid[i], NULL, philosopher, (void *)&(philosopher_nums[i])); // check i value
    }

    for (int i = 0; i < 5; i++)
    {
        pthread_join(tid[i], NULL);
    }

    return 0;
}