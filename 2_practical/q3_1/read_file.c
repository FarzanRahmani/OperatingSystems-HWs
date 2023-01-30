#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

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
		char *my_args[3];
		my_args[0] = "cat";
		my_args[1] = argv[1]; // file_name.txt as argument
		my_args[2] = NULL; 		// Indicating end of array
		execvp(my_args[0], my_args);
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
