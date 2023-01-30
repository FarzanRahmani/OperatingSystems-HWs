#include "stdlib.h"
#include "stdio.h"
#include "unistd.h"
#include "string.h"
#include "pthread.h" // gcc -pthread -o term term.c
#include <assert.h>
#include <semaphore.h>

pthread_cond_t cond;
pthread_mutex_t mutex;
int current_time; // shared data between threads (critical section )

void tick()
{
    pthread_mutex_lock(&mutex);
    current_time++;
    printf("Current time: %d\n", current_time);
    pthread_mutex_unlock(&mutex);
    // pthread_cond_signal(&cond); // signal to leading thread waiting on cond variable queue
    pthread_cond_broadcast(&cond); // signal to all threads waiting on cond variable queue (wake up all threads)
}

void *simulate_time(void *arg)
{
    while (1)
    {
        tick();
        sleep(1);
    }
}

void *alarm_clock(void *arg)
{
    pthread_mutex_lock(&mutex);
    int *input = (int *)arg;
    int delay = *input;
    printf("Alarm with delay %d is set.\n", delay);
    int alarm_time = current_time + delay;
    while (current_time < alarm_time)
    {
        pthread_cond_wait(&cond, &mutex);
        // pthread_mutex_unlock(&mutex);
        // wait for signal
        // pthread_mutex_lock(&mutex);
        // continue execution; wake up thread
    }
    printf("Alarm! in time:%d\n",current_time);
    pthread_mutex_unlock(&mutex);
}

int main(int argc, char *argv[])
{
    // Alarm Clock using monitor and condition variables

    pthread_cond_init(&cond, NULL);
    pthread_mutex_init(&mutex, NULL);
    current_time = 0;

    pthread_t tick_thread;
    pthread_t delay_thread[3];
    int delay_time1 = 2;
    int delay_time2 = 6;
    int delay_time3 = 10;

    pthread_create(&delay_thread[0], NULL, alarm_clock, &delay_time1);
    pthread_create(&tick_thread, NULL, simulate_time, NULL);
    sleep(1);
    pthread_create(&delay_thread[1], NULL, alarm_clock, &delay_time2);
    sleep(2);
    pthread_create(&delay_thread[2], NULL, alarm_clock, &delay_time3);
    
    pthread_join(delay_thread[0], NULL);
    pthread_join(delay_thread[1], NULL);
    pthread_join(delay_thread[2], NULL);
    pthread_join(tick_thread, NULL);

    pthread_cond_destroy(&cond);
    pthread_mutex_destroy(&mutex);
    
    return 0;
}