#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <stdbool.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

// Compile_q1:
// 	gcc q1.c -o q1

// Run_q1:
// 	./q1

// clean:
// 	rm q1

typedef struct prime
{
    int value;
    int counter;
} prime_t;

void run(int NUMBER_OF_PROCESSES, int NUMBER_OF_PRIMES, int *file_descriptor, int *parent_file_descriptor, prime_t **primes)
{
    for (long i = 0; i < NUMBER_OF_PRIMES; i++)
    {
        prime_t *p = primes[i];
        int pid = p->value % NUMBER_OF_PROCESSES;
        int n = write(file_descriptor[2 * pid + 1], p, sizeof(prime_t));
    }
    prime_t p;
    for (long i = 0; i < 100; i++)
    {
        int n = read(parent_file_descriptor[0], &p, sizeof(prime_t));
        printf("One phase done %d\n", p.value);
    }
}

void apply(int signum)
{
    int n = getpid();
    printf("Process with pid: %d was interrupted\n", n);
    exit(0);
}

bool IsPrime(int n)
{
    for (int i = 2; i * i <= n; i++)
        if (n % i == 0)
            return false;
    return true;
}

void clear(int NUMBER_OF_PROCESSES, int NUMBER_OF_PRIMES, prime_t **primes, int *children_processes)
{
    for (long i = 0; i < NUMBER_OF_PROCESSES; i++)
        kill(children_processes[i], SIGINT);

    for (long i = 0; i < NUMBER_OF_PRIMES; i++)
        free(primes[i]);

    free(primes);
}

int main(int argc, char const *argv[])
{
    const int NUMBER_OF_PROCESSES = 8;
    const int NUMBER_OF_PRIMES = 100;
    int parent_file_descriptor[2];
    int file_descriptor[2 * NUMBER_OF_PROCESSES];
    int children_processes[NUMBER_OF_PROCESSES];

    pipe(parent_file_descriptor);

    for (long i = 0; i < NUMBER_OF_PROCESSES; i++)
        pipe(&file_descriptor[2 * i]);

    for (long i = 0; i < NUMBER_OF_PROCESSES; i++)
    {
        children_processes[i] = fork();
        if (children_processes[i] == 0)
        {
            signal(SIGINT, apply);
            prime_t p;
            while (2 == 2)
            {
                int n = read(file_descriptor[2 * i], &p, sizeof(prime_t));
                p.value += i;
                p.counter--;
                int nextpid = 2 * ((i + 1) % NUMBER_OF_PROCESSES);
                if (p.counter == 0)
                {
                    n = write(parent_file_descriptor[1], &p, sizeof(prime_t));
                }
                else
                {
                    n = write(file_descriptor[nextpid + 1], &p, sizeof(prime_t));
                }
            }
            return 0;
        }
    }

    int i = 3;
    int count = 1;
    prime_t **primes = malloc(sizeof(prime_t *) * NUMBER_OF_PRIMES);
    primes[count - 1] = malloc(sizeof(prime_t));
    primes[count - 1]->value = 2;
    primes[count - 1]->counter = 2;
    while (count != NUMBER_OF_PRIMES)
    {
        if (IsPrime(i))
        {
            primes[count] = malloc(sizeof(prime_t));
            primes[count]->value = i;
            primes[count]->counter = i;
            count++;
        }
        i += 2;
    }

    run(NUMBER_OF_PROCESSES, NUMBER_OF_PRIMES, file_descriptor, parent_file_descriptor, primes);
    clear(NUMBER_OF_PROCESSES, NUMBER_OF_PRIMES, primes, children_processes);

    return 0;
}
