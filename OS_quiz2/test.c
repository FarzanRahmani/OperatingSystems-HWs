#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    // if(-1) --> true
    // if(0) --> false
    // if(1) --> true
    // !5 --> 0
    // !-5 --> 0
    // !0 --> 1
    int maint = getpid();
    printf("i am main %d\n",maint);
    int r = (fork() && fork());
    printf("i am %d and my parent is %d and r is: %d\n",getpid(), getppid(), r);
    // printf("\n");
    printf("hello\n");
    // if (getpid() == maint)
    // {
    //     system("pstree");
    //     printf("--------------------");
    //     system("ps -ef | grep a.out");
    // }
    // else
    // {
    //     sleep(1);
    // }
    
    return 0;
}