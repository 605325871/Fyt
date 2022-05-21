#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
int main(int argc, char* argv[])
{
    int sfd,ret;
    sfd=dup(STDOUT_FILENO);  //保存屏幕输出的文件描述符，用于恢复
    printf("sfd= %d\n",sfd);
    int fd=open("text.txt", O_CREAT|O_RDWR|O_TRUNC, S_IRUSR|S_IWUSR);
    if(fd < 0)
    {

        printf("Open Error!\n");
        return 0;
    }
    printf("-------\n");
    dup2(fd,STDOUT_FILENO);  //修改默认屏幕输出指向
    printf("write to text by STDOUT_FILENO\n");
    dup2(sfd,STDOUT_FILENO);  // 还原屏幕输出默认文件描述符指向
    printf("STDOUT_FILENO come back\n");

    return 0;
}

