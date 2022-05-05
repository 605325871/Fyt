# 线程同步的几种方式

#互斥锁
>使用互斥量完成对临界区的资源的加锁操作，使得同一时刻，对一个共享数据的使用只能又一个线程完成

例向屏幕上一次打印abcd四个字母
可以使用的是一个类似锁连的思想 a 加完解开后拿b锁依次类推
```c
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
    //创建线程
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
```

# 条件变量
条件变量并不是锁而是一种阻塞机制，使得我们的程序在某些特定的条件，比如生产者生产达到上限未消费，此时使用条件变量（加上while对条件的判断）来阻塞生产，让生产者消费

```c
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<stdlib.h>
#include<string.h>
int begnum=0;
static pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t  cond = PTHREAD_COND_INITIALIZER;
typedef struct _prodinfo
{
    int num;
    struct _prodinfo *next;
}prod;
struct _prodinfo* head=NULL;
/*  条件变量可以引起阻塞并非锁
    
*/
void *thr_produce(void*arg)
{
    while(1)
    {
        prod* pd = malloc(sizeof(struct _prodinfo));
        pd->num=begnum++;

        pthread_mutex_lock(&mut);

        pd->next=head;
        head=pd;
        printf(" -%ld号线程生产%d产品\n",pthread_self(),pd->num);
        pthread_mutex_unlock(&mut);
        pthread_cond_signal(&cond);  
        sleep(rand()%4);
    }
}

void* thr_con(void* arg)
{
    prod* pro=NULL;
    while(1)
    {
        pthread_mutex_lock(&mut);
        while(head==NULL)
            pthread_cond_wait(&cond,&mut);
        pro = head;
        head=head->next;
        printf(" -%ld号线程消费%d产品\n",pthread_self(),pro->num);
        pthread_mutex_unlock(&mut);
        free(pro);
        sleep(rand()%4);
    }
}

int main()
{
    pthread_t cid,pid;

    int err1=pthread_create(&pid,NULL,thr_produce,NULL);
     if(err1)
        {
            fprintf(stderr,"pthread_creat():%s\n",strerror(err1));
            exit(1);
        }
    int err2=pthread_create(&cid,NULL,thr_con,NULL);
      if(err2)
        {
            fprintf(stderr,"pthread_creat():%s\n",strerror(err1));
            exit(1);
        }

    pthread_join(pid,NULL);
    pthread_join(cid,NULL);
}
```
# 信号量
介绍以下信号量是进化版的互斥量，允许多个线程访问共享资源与条件变量和互斥量类此的操作，在进程和线程中均可以使用

       int sem_init(sem_t *sem, int pshared, unsigned int value);
       int sem_destroy(sem_t *sem);

       Link with -pthread.
>
* sem为定义的信号量，传出型参数
* pshared
    * 0 代表线程信号量
    * 1 代表进程信号量
* alue 为定义的信号量个数

```c
    int sem_wait(sem_t *sem);
    int sem_trywait(sem_t *sem);
    int sem_timedwait(sem_t *sem, const struct timespec *abs_timeout);

```
申请信号量，申请成功value--，当value为0