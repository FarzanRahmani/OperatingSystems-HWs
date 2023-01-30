#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <errno.h>
#include <semaphore.h>
#include "DistributedMergeSort.h"

void swap(int* a,int* b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

struct shared_memory
{
    // int *array;
    int array[MAX_ARRAY_SIZE];
    int n;
    int num_of_child_processes;
    int flag;
    int ready;
};

int main(int argc, char const *argv[]) {
    
    int shm_fd; /* shared memory file descriptor */
    struct shared_memory *ptr; /* pointer to shared memory object */
    sem_t *sem;

    sem = sem_open(SEM_NAME, O_RDONLY, 0666, 1); // 0666 is the permission
    shm_fd = shm_open(SHM_NAME, O_RDWR, 0666); // 0666 is the permission (Open shared memory segment.)
    if (shm_fd < 0)
    {
        perror("Failed to open shared mem");
        return EXIT_FAILURE;
    }
    
    // int err = ftruncate(shm_fd, sizeof(struct shared_memory)); // Resize the shared memory segment to the size of the shared memory structure.
    // if (err < 0)
    // {
    //     perror("Failed to resize shared mem");
    //     return EXIT_FAILURE;
    // }

    ptr = (struct shared_memory *)mmap(0, sizeof(struct shared_memory), PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0); // Map the shared memory segment to the address space of the process.
    if (ptr == MAP_FAILED)
    {
        perror("Failed to map shared mem");
        return EXIT_FAILURE;
    }

    sem_wait(sem);
    ptr->flag = 0;
    sem_post(sem);

    // wait for mother process to finish writing to shared memory
    // while (ptr->ready == 0)
    // {
    // }
    int test = 0;
    do
    {
        sem_wait(sem);
        test = ptr->ready;
        sem_post(sem);
    } while (test == 0);
    
    
    pid_t pids[ptr->num_of_child_processes];
    int num_of_child_processes = ptr->num_of_child_processes;
    int array_size = ptr->n;
    // sort the slices of array
    for (int i = 0; i < num_of_child_processes; i++)
    {
        int pid = fork();
        if (pid == 0) {
            int slice_unit = array_size / num_of_child_processes;
            int start = i * slice_unit;
            int end = (i + 1) * slice_unit;
            for (int j = start; j < end; j++)
            {
                printf("j: %d %d \n", j, ptr->array[j]);
                for (int k = j + 1; k < end; k++) // selection sort
                {
                    if (ptr->array[j] > ptr->array[k])
                    {
                        sem_wait(sem);
                        swap(&(ptr->array[j]), &(ptr->array[k]));
                        sem_post(sem);
                        // int temp = ptr->array[j];
                        // ptr->array[j] = ptr->array[k];
                        // ptr->array[k] = temp;
                    }
                }
            }
            exit(0);
        }
        else if (pid > 0) {
            // parent process
            pids[i] = pid;
        }
        else {
            perror("Error in creating child process.");
            return EXIT_FAILURE;
        }

    }
    
    // wait for all child processes to complete
    for (int i = 0; i < ptr->num_of_child_processes; i++)
    {
        waitpid(pids[i], NULL, 0);
    }
    ptr->flag = 1;

    // free(ptr->array);
    munmap(ptr, sizeof(struct shared_memory));
    close(shm_fd);
    sem_close(sem);
    sem_unlink(SEM_NAME);
    shm_unlink(SHM_NAME);
    return 0;
}