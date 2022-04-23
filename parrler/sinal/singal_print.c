# include <stdio.h>
# include<stdlib.h>
# include <signal.h>
# include <unistd.h>
static void int_handler(int s)
{
    write(1,"1",1);
}
int main()
{
    //在程序收到信号时，捕获信号
    signal(SIGINT, int_handler); //信号会打断阻塞的系统调用  // 信号单次响应后会从指定行为，变为默认行为
    for(int i=0 ; i < 10 ;i++)
    {
        write(1,"*",1);
        sleep(1);
    }
}