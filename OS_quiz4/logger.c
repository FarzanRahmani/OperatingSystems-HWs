#include <sys/mman.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

#define LOGFILE "/tmp/farzan.log"

#define MAXIMUM_BUFFERS 20

struct shared_memory
{
    char buf[MAXIMUM_BUFFERS][300];
    int buffer_idx;
    int buffer_print_idx;
};

int main(int argc, char **argv)
{
    const char *SHARED_MEMORY_NAME = "/posix-shared-memory-example";
    const char *SEMAPHORE_MUTEX_NAME = "/semaphore-mutex";
    const char *SEMAPHORE_SPOOL_SIGNAL_NAME = "/semaphore-spool-signal";
    const char *SEMAPHORE_BUFFER_COUNT_NAME = "/semaphore-buffer-count";

    struct shared_memory *shared_memory_pointer;
    sem_t *mutex_semaphore;
    sem_t *spool_signal_semaphore;
    sem_t *buffer_count_semaphore;
    int shm_fd;
    int fd_log;
    char farzan_buf[300];

    if ((fd_log = open(LOGFILE, O_CREAT | O_WRONLY | O_APPEND | O_SYNC, 0666)) == -1)
    {
        printf("error: fopen failed");
        exit(1);
    }

    if ((mutex_semaphore = sem_open(SEMAPHORE_MUTEX_NAME, O_CREAT, 0660, 0)) == SEM_FAILED)
    {
        printf("error: sem_open failed");
        exit(1);
    }

    // Get shared memory
    if ((shm_fd = shm_open(SHARED_MEMORY_NAME, O_RDWR | O_CREAT | O_EXCL, 0660)) == -1)
    {
        printf("error: shm_open failed");
        exit(1);
    }

    if (ftruncate(shm_fd, sizeof(struct shared_memory)) == -1)
    {
        printf("error: ftruncate failed");
        exit(1);
    }

    if ((shared_memory_pointer = mmap(NULL, sizeof(struct shared_memory), PROT_READ | PROT_WRITE, MAP_SHARED,
                                      shm_fd, 0)) == MAP_FAILED)
    {
        printf("error: mmap failed");
        exit(1);
    }

    shared_memory_pointer->buffer_idx = shared_memory_pointer->buffer_print_idx = 0;

    if ((buffer_count_semaphore = sem_open(SEMAPHORE_BUFFER_COUNT_NAME, O_CREAT | O_EXCL, 0660, MAXIMUM_BUFFERS)) == SEM_FAILED)
    {
        printf("error: sem_open failed");
        exit(1);
    }

    if ((spool_signal_semaphore = sem_open(SEMAPHORE_SPOOL_SIGNAL_NAME, O_CREAT | O_EXCL, 0660, 0)) == SEM_FAILED)
    {
        printf("error: sem_open failed");
        exit(1);
    }

    if (sem_post(mutex_semaphore) == -1)
    {
        printf("error: sem_post failed");
        exit(1);
    }

    for (;;)
    {
        if (sem_wait(spool_signal_semaphore) == -1)
        {
            printf("error: sem_wait failed");
            exit(1);
        }

        strcpy(farzan_buf, shared_memory_pointer->buf[shared_memory_pointer->buffer_print_idx]);

        shared_memory_pointer->buffer_print_idx = shared_memory_pointer->buffer_print_idx + 1;
        if (shared_memory_pointer->buffer_print_idx == MAXIMUM_BUFFERS)
            shared_memory_pointer->buffer_print_idx = 0;

        if (sem_post(buffer_count_semaphore) == -1)
        {
            printf("error: sem_post failed");
            exit(1);
        }

        if (write(fd_log, farzan_buf, strlen(farzan_buf)) != strlen(farzan_buf))
        {
            printf("error: write failed");
            exit(1);
        }
    }

    return 0;
}
