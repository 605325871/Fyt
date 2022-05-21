#include <stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<string.h>
#include <sched.h>
#define left 30000000
#define right 30000200
#define SIZE 4

static int num = 0;
static pthread_mutex_t mut_num = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond_num=PTHREAD_COND_INITIALIZER; //初始化条件变量

void *thr_prime(void *p)
{
    int i;
    while (1)
    {  
    pthread_mutex_lock(&mut_num);
    while(num == 0) 
    {
        pthread_cond_wait(&cond_num,&mut_num);
    }
    if(num == -1)
    {  
        pthread_mutex_unlock(&mut_num);
        break;
    }             // 要注意临界区内的所有跳转语句，比如如果 直接break 则是在被加锁状态下，会一直阻塞
    i = num;
    num = 0;
    pthread_cond_broadcast(&cond_num);
    pthread_mutex_unlock(&mut_num);
    for( i=left;i<=right;i++)
    {
       int mark=1;
       for(int j = 2; j<i/2; j++)
       {
           if(i%j ==0)
           {
               mark = 0;
               break;
           }
       }
       if(mark)
        printf("【 %d 】%d is a primer\n",(int )p,i);
    }
   }
   pthread_exit(NULL);
}

int main()
{
    int i,j,k;
    int err;
    pthread_t tid[SIZE];
    //main 线程 进行创建线程
    for(i=0 ; i<=SIZE ;i++)
    {
        err =pthread_create(tid+i ,NULL,thr_prime,(void*)i);
        if(err)
        {
            fprintf(stderr,"pthread_creat():%s\n",strerror(err));
            exit(1);
        }
    }
    for ( i = left; i <=right; i++)
    {
        pthread_mutex_lock(&mut_num); // 加锁之后发现
        while(num != 0) // NUM不为0 任务没有被抢走
        {
        pthread_cond_wait(&cond_num,&mut_num);
        }
        num = i;
        pthread_cond_signal(&cond_num);
        pthread_mutex_unlock(&mut_num);
    }

    pthread_mutex_lock(&mut_num);   
      while(num != 0)  //防止最后一个任务被覆盖
    {
        pthread_mutex_unlock(&mut_num);
         sched_yield();
        pthread_mutex_lock(&mut_num);
    }

    num = -1;
 pthread_cond_wait(&cond_num,&mut_num);
    pthread_mutex_unlock(&mut_num);
    //为线程收尸
    for(i =0 ; i<=SIZE ;i++)
        pthread_join(tid[i],NULL);

    pthread_mutex_destroy(&mut_num);
    pthread_cond_destroy(&cond_num);  //销毁条件变量
    exit(0);

}