#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<stdlib.h>
#include<string.h>
pthread_rwlock_t rwlock =PTHREAD_RWLOCK_INITIALIZER;
int beginum=100;
void*thr_Wr(void*arg)
{
    while(1)
    {
        pthread_rwlock_wrlock(&rwlock);
        printf("-写线程--beginum = %d\n",beginum++);
        usleep(2000);//模拟占用时间
        pthread_rwlock_unlock(&rwlock);
        usleep(2000);//简单防止再抢锁的方法但不建议使用
    }
    return NULL;
}
void*thr_ead(void*arg)
{
    while (1)
    {
        pthread_rwlock_rdlock(&rwlock);
      printf("-读读线程--beginum = %d\n",beginum);
        usleep(2000);//模拟占用时间
       pthread_rwlock_unlock(&rwlock);
        usleep(2000);//简单防止再抢锁的方法但不建议使用

    }
    
    return NULL;
}
int main()
{
    int n=8,i=0;
    pthread_t tid[8];


    for(i = 0; i<5;i++)
    {
        pthread_create(&tid[i],NULL,thr_ead,NULL);
    }
     for(; i<8;i++)
    {
        pthread_create(&tid[i],NULL,thr_Wr,NULL);
    }
     for(i = 0; i<8;i++)
    {
        pthread_join(tid[i],NULL);
    }

   pthread_rwlock_destroy(&rwlock);
}