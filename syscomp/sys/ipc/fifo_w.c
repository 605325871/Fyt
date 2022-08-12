#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>
int main(int argc ,char ** argv)
{
    if(argc != 2)
    {
        printf("./a.out fifoname\n");
        exit(-1);
    }
    //当前目录有一个myfifo文件  打开myfifo文件 filo 只有读和写的时候open 打开fifo文件时  
  // open注意事项 打开fifo文件时，read端会阻塞等待write端open，write端同理，也会等待阻塞
  
    int fd = open(argv[1],O_WRONLY);
    char buf[256];
    int num;
    while(1)
    {
        //当前目录有一个myfifo文件
        memset(buf,0x00,sizeof(buf));
        sprintf(buf,"xiaoming%04d\n",num++);
        write(fd,buf,sizeof(buf));
    }
     close(fd);
        return 0;

}