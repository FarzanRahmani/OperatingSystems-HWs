#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
// https://www.geeksforgeeks.org/fork-practice-questions/
// int main(int argc, char *argv[])
// {
//     // if(-1) --> true
//     // if(0) --> false
//     // if(1) --> true
//     // !5 --> 0
//     // !-5 --> 0
//     // !0 --> 1
//     printf("i am main %d\n", getpid());
//     if(fork() || (!fork())) // 3 state --> 1, 0, 1 
//     // OR operator (i.e, ||) evaluate second condition when first condition is false.
//     {
//         printf("first block: i am %d and my parent is %d\n", getpid(), getppid());
//         if(fork() && fork()) // 3 state --> 1, 0, 0
//         // AND operator (i.e, &&) if first condition is false then it will not evaluate second condition
//         {
//             printf("second block: i am %d and my parent is %d\n", getpid(), getppid());
//             fork();
//         }
//     }
//     printf("\n");
//     // printf("hello\n");
//     return 0;
    
// }

int main(int argc, char *argv[])
{
    if(fork() || (!fork()))
    {
        if(fork() && fork())
        {
            fork();
        }
    }
    printf("hello\n");
    return 0;
}