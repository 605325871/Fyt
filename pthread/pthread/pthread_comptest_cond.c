#include <stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<string.h>
#define THRNUM 4

static pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond=PTHREAD_COND_INITIALIZER;
static int num=0;
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
    pthread_mutex_lock(&mut);
    while(num!=n)
            pthread_cond_wait(&cond,&mut);

    write(1,&c,1);
    num = next(num);
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mut);
    }
    pthread_exit(NULL);

}
int main()
{
    int i,err;
    pthread_t tid[THRNUM];
    //创建xian程
    for(i = 0 ; i < THRNUM ;i++){
    err = pthread_create(tid+i,NULL,pthreadfunc,(void*)i );
    if(err != 0)
    {
        fprintf(stderr,"create:%s\n",strerror(err));
        exit(1);
    }
    }
    alarm(5);
    for(i = 0 ; i < THRNUM ;i++){
    pthread_join(tid[i],NULL);
    }
    pthread_mutex_destroy(&mut);
    pthread_cond_destroy(&cond);
}