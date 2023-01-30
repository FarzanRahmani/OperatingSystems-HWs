#define _GNU_SOURCE
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/uio.h>
#include <fcntl.h>
#include <limits.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

// ComplieCMA:
// 	gcc CMA.c -o CMA

// RunCMA:
// 	./CMA

// clean:
// 	rm -f CMA

int main(int argc, char const *argv[])
{
    const int GIBI = 1073741824;
    int file_descriptor[2];
    pipe(file_descriptor);

    pid_t pid = fork();
    if (pid == 0)
    {
        pid_t producer = getppid();
        char *localDT = malloc(sizeof(char) * (size_t)3 * GIBI);
        char *remoteDT;
        size_t batch_size = ((size_t)3 * GIBI) / IOV_MAX;
        size_t n = read(file_descriptor[0], &remoteDT, sizeof(char *));

        struct iovec local[IOV_MAX];

        for (int i = 0; i < IOV_MAX; i++)
        {
            local[i].iov_base = &localDT[i * batch_size];
            local[i].iov_len = sizeof(char) * batch_size;
        }

        n = 0;
        int offset = IOV_MAX / 8;

        for (int i = offset - 1; i < IOV_MAX; i += offset)
        {
            int difference = 0;
            if (i - offset >= 0)
            {
                difference = i - offset;
            }
            else
            {
                difference = 0;
            }
            n += process_vm_readv(producer, &local[difference], offset, &local[difference], offset, 0);
            printf("Consumer read %ld bytes\n", n);
        }

        free(remoteDT);
        return 0;
    }
    
    char *my_data = malloc(sizeof(char) * ((size_t)3 * GIBI));
    size_t f = ((size_t)3 * GIBI) / 8;
    printf("%ld\n", f);
    char str[] = {'F', 'A', 'R', 'Z', 'A', 'N', 'R', 'M'};
    for (size_t i = 0; i < 8; i++)
    {
        memset(my_data + sizeof(char) * f * i, str[i], sizeof(char) * f);
    }

    int n = write(file_descriptor[1], &pid, sizeof(pid_t));
    n = write(file_descriptor[1], &my_data, sizeof(char *));
    wait(NULL);
    free(my_data);

    return 0;
}
