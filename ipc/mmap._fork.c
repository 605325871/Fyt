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

    //x 先打开文件
    int fd = open("2.txt",O_RDWR);
   //创建映射区
    int *mem = mmap(NULL,4,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
   //映射区的释放与文件关闭无关。只要映射建立成功，文件可以立即关闭。
   close(fd);
   //fork子进程
    pid_t pid = fork();
    
    if (pid == 0)
    {
        *mem = 100;
        printf("child *mem = %d\n",*mem);
        sleep(3);
         printf("child *mem = %d\n",*mem);
    }
    else
    {
         sleep(1
         );
         printf("parent *mem = %d\n",*mem);
         *mem = 1001;
         printf("parent *mem = %d\n",*mem);
         wait(NULL);
    }
    

    munmap(mem,4);
}