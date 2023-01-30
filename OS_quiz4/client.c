#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdio.h>
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
    sem_t *buffer_count_semaphore;
    sem_t *mutex_semaphore;
    sem_t *spool_signal_semaphore;
    int shm_fd;

    if ((mutex_semaphore = sem_open(SEMAPHORE_MUTEX_NAME, 0, 0, 0)) == SEM_FAILED)
    {
        printf("error: sem_open failed");
        exit(1);
    }

    if ((shm_fd = shm_open(SHARED_MEMORY_NAME, O_RDWR, 0)) == -1)
    {
        printf("error: shm_open failed");
        exit(1);
    }

    if ((shared_memory_pointer = mmap(NULL, sizeof(struct shared_memory), PROT_READ | PROT_WRITE, MAP_SHARED,
                                      shm_fd, 0)) == MAP_FAILED)
    {
        printf("error: mmap failed");
        exit(1);
    }

    if ((buffer_count_semaphore = sem_open(SEMAPHORE_BUFFER_COUNT_NAME, 0, 0, 0)) == SEM_FAILED)
    {
        printf("error: sem_open failed");
        exit(1);
    }

    if ((spool_signal_semaphore = sem_open(SEMAPHORE_SPOOL_SIGNAL_NAME, 0, 0, 0)) == SEM_FAILED)
    {
        printf("error: sem_open failed");
        exit(1);
    }

    char buf[282], *cp;

    printf("Type a message: ");

    while (fgets(buf, 198, stdin))
    {
        int length = strlen(buf);
        if (buf[length - 1] == '\n')
            buf[length - 1] = '\0';

        if (sem_wait(buffer_count_semaphore) == -1)
        {
            printf("error: buffer_count_semaphore failed");
            exit(1);
        }

        if (sem_wait(mutex_semaphore) == -1)
        {
            printf("error: mutex_semaphore failed");
            exit(1);
        }

        // Critical section
        time_t now = time(NULL);
        cp = ctime(&now);
        int len = strlen(cp);
        if (*(cp + len - 1) == '\n')
            *(cp + len - 1) = '\0';
        sprintf(shared_memory_pointer->buf[shared_memory_pointer->buffer_idx], "<client pid:%d><timestamp:%s><message:%s>\n", getpid(),
                cp, buf);
        shared_memory_pointer->buffer_idx = shared_memory_pointer->buffer_idx + 1;
        if (shared_memory_pointer->buffer_idx == MAXIMUM_BUFFERS)
            shared_memory_pointer->buffer_idx = 0;

        if (sem_post(mutex_semaphore) == -1)
        {
            printf("error: mutex_semaphore failed");
            exit(1);
        }

        if (sem_post(spool_signal_semaphore) == -1)
        {
            printf("error: spool_signal_semaphore failed");
            exit(1);
        }

        printf("Type a message: ");
    }

    if (munmap(shared_memory_pointer, sizeof(struct shared_memory)) == -1)
    {
        printf("error: munmap failed");
        exit(1);
    }

    close(shm_fd);
    sem_close(mutex_semaphore);
    sem_close(buffer_count_semaphore);
    sem_close(spool_signal_semaphore);
    sem_unlink(SEMAPHORE_MUTEX_NAME);
    sem_unlink(SEMAPHORE_BUFFER_COUNT_NAME);
    sem_unlink(SEMAPHORE_SPOOL_SIGNAL_NAME);

    return 0;
}