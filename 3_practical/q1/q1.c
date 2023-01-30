#include "stdlib.h"
#include "stdio.h"
#include "unistd.h"
#include "string.h"
#include "pthread.h" // gcc -pthread -o term term.c
#include <assert.h>

typedef struct {int start; int end;} myarg_t;
typedef struct {double ret;} myret_t;

void *calculate_partial_pi(void *arg)
{
    myarg_t *args = (myarg_t *) arg;
    int start = args->start;
    int end = args->end; // (*args).end;
    double pi = 0.0;
    for (int i = start; i < end; i++)
    {
        if (i % 2 == 0)
        {
            pi += 1.0 / (2 * i + 1);
        }
        else
        {
            pi -= 1.0 / (2 * i + 1);
        }
    }
    myret_t *rvals = malloc(sizeof(myret_t));
    assert(rvals != NULL);  // if conditon is True => return error
    rvals->ret = pi;
    return (void *) rvals;
}

int main(int argc, char *argv[])
{
    // calulate pi using series formula
    int num_of_terms = 0;
    int num_of_threads = 0;
    printf("Enter the number of terms: ");
    scanf("%d", &num_of_terms);
    printf("Enter the number of threads: ");
    scanf("%d", &num_of_threads);
    assert(num_of_terms > 0);
    assert(num_of_threads > 0);
    assert(num_of_terms >= num_of_threads);

    int terms_per_thread = num_of_terms / num_of_threads;
    int remainder = num_of_terms % num_of_threads;
    double pi = 0.0;

    pthread_t *threads = malloc(sizeof(pthread_t) * num_of_threads);
    assert(threads != NULL);
    myarg_t *args = malloc(sizeof(myarg_t) * num_of_threads);
    assert(args != NULL);
    for (int i = 0; i < num_of_threads; i++)
    {
        args[i].start = i * terms_per_thread;
        args[i].end = (i + 1) * terms_per_thread;
        if (i == num_of_threads - 1) // last thread
        {
            args[i].end += remainder;
        }
        pthread_create(&threads[i], NULL, calculate_partial_pi, &args[i]);
    }

    myret_t *rvals; // return values of thread
    for (int i = 0; i < num_of_threads; i++)
    {
        pthread_join(threads[i], (void **) &rvals);
        pi += rvals->ret; // return value of thread
        free(rvals);
    }

    pi *= 4.0;
    printf("pi = %.10f\n", pi);
    free(threads);
    free(args);
    return 0;
}