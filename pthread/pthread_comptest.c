#include <stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<string.h>
#define THRNUM 4

static pthread_mutex_t mut[4];
static int next(int n)
{
    if(n + 1 == THRNUM)
    return 0;
    return n+1;
}
static void* pthreadfunc(void* p)
{
    int n =(int)p;
    char c = 'a' + (int)p;

    while(1)
    {
    pthread_mutex_lock(mut + n);
    write(1,&c,1);
    pthread_mutex_unlock(mut + next(n));
    }
    pthread_exit(NULL);

}
int main()
{
    int i,err;
    pthread_t tid[THRNUM];
    //创建xian程
    for(i = 0 ; i < THRNUM ;i++){
        //初始化锁
        pthread_mutex_init(mut + i,NULL);
        //加锁
        pthread_mutex_lock(mut+i );
    err = pthread_create(tid+i,NULL,pthreadfunc,(void*)i );
    if(err != 0)
    {
        fprintf(stderr,"create:%s\n",strerror(err));
        exit(1);
    }
    }
    //回收线程
    pthread_mutex_unlock(mut + 0);
    alarm(5);
    for(i = 0 ; i < THRNUM ;i++){
    pthread_join(tid+i,NULL);
    }
}