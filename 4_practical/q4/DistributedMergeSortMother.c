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

void Merge(int arr[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1; // size of L (left array)
    int n2 = r - m; // size of R (right array)
    int L[n1], R[n2];
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];
    i = 0; // for L (left)
    j = 0; // for R (right)
    k = l; // for arr (merged)
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
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
    // 1 mother process, 2^n child processes

    int shm_fd; /* shared memory file descriptor */
    struct shared_memory *ptr; /* pointer to shared memory object */
    sem_t *sem;

    sem = sem_open(SEM_NAME, O_CREAT, 0666, 1); // 0666 is the permission
    shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666); // 0666 is the permission (Open shared memory segment.)
    if (shm_fd < 0)
    {
        perror("Failed to open shared mem");
        return EXIT_FAILURE;
    }
    
    int err = ftruncate(shm_fd, sizeof(struct shared_memory)); // Resize the shared memory segment to the size of the shared memory structure.
    if (err < 0)
    {
        perror("Failed to resize shared mem");
        return EXIT_FAILURE;
    }

    ptr = (struct shared_memory *)mmap(0, sizeof(struct shared_memory), PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0); // Map the shared memory segment to the address space of the process.
    if (ptr == MAP_FAILED)
    {
        perror("Failed to map shared mem");
        return EXIT_FAILURE;
    }
    sem_wait(sem);
    ptr->flag = 0;
    ptr->ready = 0;
    sem_post(sem);

    printf("Distributed Merge Sort Mother\n");
    printf("Enter the number of processes (should be a power of two):");
    int num_of_child_processes;
    scanf("%d", &num_of_child_processes);
    sem_wait(sem);
    ptr->num_of_child_processes = num_of_child_processes;
    sem_post(sem);
    int n = 0;
    printf("Enter the number of elements in the array: ");
    scanf("%d", &n);
    sem_wait(sem);
    ptr->n = n;
    sem_post(sem);
    // ptr->array = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
    {
        printf("Enter the element %d: ", i);
        sem_wait(sem);
        scanf("%d", &(ptr->array[i]));
        sem_post(sem);
    }

    sem_wait(sem);
    ptr->ready = 1;
    sem_post(sem);

    // sleep(5); // wait for the child processes to finish
    // while (! ptr->flag)
    // {
        
    // }
    int test = 0;
    do
    {
        sem_wait(sem);
        test = ptr->flag;
        sem_post(sem);
    } while (test == 0);
    

    for (int i = 0; i < num_of_child_processes - 1; i++)
    {
        Merge(ptr->array, 0, (i + 1) * (n / num_of_child_processes) - 1, (i + 2) * (n / num_of_child_processes) - 1);
    }
    
    // print sorted array
    for (int i = 0; i < n; i++)
    {
        printf("%d ", ptr->array[i]);
    }
    printf("\n");

    // free(ptr->array);
    munmap(ptr, sizeof(struct shared_memory));
    close(shm_fd);
    sem_close(sem);
    sem_unlink(SEM_NAME);
    shm_unlink(SHM_NAME);
    return 0;
}