#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<sys/mman.h>
#include<stdlib.h>
#include<sys/wait.h>
int main()
{
    int *mem = mmap(NULL,4,PROT_WRITE|PROT_READ,MAP_SHARED|MAP_ANON,-1,0);

    if( mem ==MAP_FAILED )
    {
        perror("mmap err");
        exit(-1);
    }

    pid_t pid = fork();

    if( pid == 0)
    {
         *mem = 100;
        printf("child *mem = %d\n",*mem);
        sleep(3);
         printf("child *mem = %d\n",*mem);

    }
    else
    {
         sleep(1);
         printf("parent *mem = %d\n",*mem);
         *mem = 1001;
         printf("parent *mem = %d\n",*mem);
         wait(NULL);

    }
    munmap(mem,4);
}