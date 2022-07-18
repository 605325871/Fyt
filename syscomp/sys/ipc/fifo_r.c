#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>
int main(int argc ,char ** argv)
{
        if(argc!=2)
        {
              printf("./a.out fifoname\n");
              exit(-1);
        }
        //读端关闭 写端也关闭
        int ret;
        int fd=open(argv[1],O_RDONLY);
        char buf[256];
        while (1)
        {
            memset(buf,0x00,sizeof(buf));
            ret = read(fd,buf,sizeof(buf));
            if(ret >0)
            {
                printf("read : %s\n",buf);
            }
        }
        close(fd);
        return 0;

}
