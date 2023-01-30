// https://www.geeksforgeeks.org/system-call-in-c/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>


int main(int argc, char const *argv[])
{
    // ./read_file.o file_name.txt
    // argv[0] = ./read_file
    // argv[1] = file_name.txt
    // for (int i = 0; i < argc; i++)
    // {
    //     printf("%s\n", argv[i]);
    // }

    // printf("Hello World! My pid is: %d\n", getpid());

	int r = fork();
    
	if (r < 0){
		printf("fork failed!\n");
		exit(1);
	} else if (r == 0){ 
		// Child Process
		// printf("Hello World, I am child process. My pid is: %d\n", getpid());
        char command[100];// const char *command = "cat" + " " + argv[1];
        strcpy(command, "cat ");
        strcat(command, argv[1]);
        system(command);
        exit(0);
	} else{
		// Parent Process
		// int w = wait(NULL);
        int stat;
        pid_t cpid = waitpid(r, &stat, 0);
		// printf("I am parent of %d. pid is: %d\n", r, getpid());
	}

    return 0;
}