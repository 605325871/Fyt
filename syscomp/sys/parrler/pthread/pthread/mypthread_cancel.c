#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<stdlib.h>
#include<string.h>

/*
线程的取消：  pthread_cancel（）；
取消有两种状态 ： 允许和不允许
允许取消又分为： 异步cancel   ， 推迟 cancel（默认） -> 推迟到cancel点
cancel点： posix定义的canceldian 都是可能引发阻塞的系统调用
pthread_setcancelstate(int ):设置是否允许取消
pthread_setcanceltype(); 设置取消方式 异步还是推迟

void pthread_testcancel(void); 本函数什么都不做取消点

线程分离
*/

void* thread1 (void* arg)
{
    printf("%s returning..\n",(char*)arg);
    int *p=(int*)malloc(sizeof(int));
    *p=1;
    return (void*)p;
}
void *thread2(void *arg)
{
    printf("%s exiting ...\n",(char*)arg);
    int *p = (int*)malloc(sizeof(int));
    *p = 2;
    pthread_exit((void*)p);
} 
void *thread3(void *arg)
{
    while ( 1 ){ //
        printf("%s is running ...\n",(char*)arg);
        sleep(1);
    } 
    return NULL;
} 
int main(void)
{
    pthread_t tid;
    void *ret;
    //thread 1 return
    pthread_create(&tid,NULL,thread1,(void*)"thread 1");
    pthread_join(tid,&ret);
    printf("thread 1 return , thread id%X,return code :%d\n",tid,*(int*)ret);
    free(ret);
    // thread 2 exit
    pthread_create(&tid, NULL, thread2, (void*)"thread 2");
    pthread_join(tid, &ret);
    printf("thread 2 exit, thread id %X, return code:%d\n", tid, *(int*)ret);
    free(ret);
    //thread 3
    pthread_create(&tid,NULL,thread3,(void*)"thread 3");
    sleep(3);
    pthread_cancel(tid);
    pthread_join(tid,&ret);

    if(ret = PTHREAD_CANCELED)
        printf("thread 3 cancel ,thread is %X,return code:pthread_Cande->%d\n",tid,ret);
      else
        printf("thread return, thread id %X, return code:%d\n", tid,ret);
    return 0;


}