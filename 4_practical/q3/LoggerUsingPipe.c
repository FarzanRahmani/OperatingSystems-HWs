#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <sys/wait.h> // for waitpid
#include <time.h>     // for time
#include <string.h>   // for strlen
#include <semaphore.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>

void writeInPipe(int write_pipe, int i)
{
    int mypid = getpid();
    sleep(0.5);
    // time_t now = time(NULL);
    // char* cp = ctime(&now);
    // int len = strlen(cp);
    // char *msg = "Hello from child ";
    // char mypid_str[15];
    // sprintf(mypid_str, "%d in time", i);
    // msg = strcat(msg, mypid_str);
    // strcat(msg, cp);
    // char pid[15];
    // sprintf(pid, "pid: %d\n", mypid);
    // msg = strcat(msg, pid);
    
    char *msg = "Hello from child ";
    // write to pipe
    write(write_pipe, msg, strlen(msg));
}

int runProducers(int pipes[4][2])
{
    for (int i = 0; i < 4; i++)
    {
        int pid = fork();
        if (pid > 0)
        {
            // parent process
        }
        else if (pid == 0)
        {
            // child process
            writeInPipe(pipes[i][1], i);
            return EXIT_SUCCESS;
        }
        else
        {
            perror("Unable to create a new process\n");
            exit(1);
        }
    }
    for (int i = 0; i < 4; i++)
    {
        wait(NULL);
    }
    
    return EXIT_SUCCESS;
}

int main(int argc, char **argv)
{
    // assume we have 4 log producer and 1 logger which write logs in file
    // we will use pipe to communicate between producer and logger
    // we will use 4 pipes to communicate between 4 producer and logger

    // create 4 pipes
    int pipes[4][2];

    // Initialize pipes
    for (int i = 0; i < 4; i++)
    {
        if (pipe(pipes[i]) == -1)
        {
            perror("pipe");
            exit(1);
        }
    }

    int pid = fork();

    if (pid > 0)
    {
        // parent process

        FILE *fp = fopen("log.txt", "a");
        // read from pipe
        for (int i = 0; i < 4; i++)
        {
            char buf[200];
            read(pipes[i][0], buf, 200);
            printf("Received: [%s] from child %d \n", buf, i);

            // write to file
            fprintf(fp, "[%s] from child %d \n", buf, i);
            sleep(1);
        }
        fclose(fp);

        printf("Done!\n");
        // close read and write end of all pipes
        for (int i = 0; i < 4; i++)
        {
            close(pipes[i][0]);
            close(pipes[i][1]);
        }

        exit(0);
    }
    else if (pid == 0)
    {
        // child process
        runProducers(pipes);
    }
    else
    {
        perror("Unable to create a new process\n");
        exit(1);
    }

    return 0;
}