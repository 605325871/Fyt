#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<semaphore.h>
#include<string.h>
#include<stdlib.h>
sem_t blank,xfull;
#define semcnt 5
int i=0;
int queue[semcnt];
int beginnum = 100;
void *thr_produce(void*arg)
{
    while(1)
    {
        sem_wait(&blank);//申请资源
        printf("%ld 线程生产了 %d\n",pthread_self(),beginnum);
        queue[(i++)%semcnt]= beginnum++;
        sem_post(&xfull);
        sleep(rand()%4);
    }
    return NULL;
}
void* thr_con(void* arg)
{   
    int i=0;
    int num=0;
    while(1)
    {
        sem_wait(&xfull);
        num = queue[(i++)%semcnt];
        printf("%ld 线程消费了 %d\n",pthread_self(),num);
        sem_post(&blank);
        sleep(rand()%3);
    }
    return NULL;
}
int main()
{
    sem_init(&blank,0,semcnt);
    sem_init(&xfull,0,0); //消费者初始默认没有产品
    pthread_t tid[2];
     int err1=pthread_create(&tid[0],NULL,thr_produce,NULL);
     if(err1)
        {
            fprintf(stderr,"pthread_creat():%s\n",strerror(err1));
            exit(1);
        }
    int err2=pthread_create(&tid[1],NULL,thr_con,NULL);
      if(err2)
        {
            fprintf(stderr,"pthread_creat():%s\n",strerror(err1));
            exit(1);
        }
        pthread_join(tid[0],NULL);
        pthread_join(tid[1],NULL);

    sem_destroy(&blank);
    sem_destroy(&xfull);

}