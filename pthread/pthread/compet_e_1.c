#include <stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<string.h>
#define thrnum 20
#define fname "/tmp/out"

#define linesize 1024
//初始化互斥量
static pthread_mutex_t mut=PTHREAD_MUTEX_INITIALIZER;

void *thr_prime(void *p)
{
    FILE *fp;
    char linebuf[linesize];
    fp =fopen(fname,"r+");   //多个线程之间相撞拿到 同一个fp 开始覆盖写操作
    if(fp == NULL)  
    {
        perror("fopen");
        exit(-1);
    }
    //加锁
    pthread_mutex_lock(&mut);
    fgets(linebuf,linesize,fp);
    fseek(fp,0,SEEK_SET);
    fprintf(fp,"%d\n",atoi(linebuf)+1);
    //解锁
    pthread_mutex_unlock(&mut);
    fclose(fp);
    pthread_exit(NULL);
  
}

int main()
{
    int err,i;
    pthread_t tid[thrnum];
    //main 线程 进行创建线程
    for(i=0 ; i<=thrnum ;i++)
    {
        err =pthread_create(tid + i ,NULL,thr_prime,NULL);
        if(err)
        {
            fprintf(stderr,"pthread_creat():%s\n",strerror(err));
            exit(1);
        }

    }
    //为线程收尸
    for(i =0 ; i<=thrnum ;i++)
    {
        pthread_join(tid[i],NULL);
    }
    pthread_mutex_destroy(&mut);
    exit(0);

}