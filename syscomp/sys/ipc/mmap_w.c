#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<sys/mman.h>
#include<stdlib.h>
#include<sys/wait.h>

typedef struct student
{
    int sid;
    char sname[20];
    
}st;
 
int main(int argc,char *argv[])
{

    if(argc != 2)
    {
        printf(" little ");
    }
    // OPEN file 文件
    int fd = open(argv[1],O_RDWR|O_CREAT|O_TRUNC,0666);

    int length = sizeof(st);

    ftruncate(fd,length);
    // 创建映射区

    st * stu = mmap(NULL,length,PROT_WRITE|PROT_READ,MAP_SHARED,fd,0);

    if(stu == MAP_FAILED)
    {
        perror("mmap err");
        exit(-1);
    }

 int num = 1;

 while(1)
 {
     stu->sid=num;
     sprintf(stu->sname,"xiaoming-%03d",num++);
     sleep(1);
 }

 munmap(stu,length);
 close(fd);
 return 0 ;
}
