#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<sys/mman.h>
#include<stdlib.h>
int main(int argc,char **argv)
{
    char buf[256];
    int fd; 
    fd= open( "2.txt",O_RDWR);
    int ret=read(fd,buf,sizeof(buf));
    //创建内存映射区
    char *mem = mmap(NULL,ret,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);

        //判断是否成功  
     if(mem == MAP_FAILED)
     {
         perror("mem");
         exit(-1);
     }

     strcpy(mem,"hello");
     munmap(mem,ret);
     close(fd);
     return 0;
}