#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
// Single-producer , single-consumer Queue
int i=0;
struct SPSCQueue *head;
static pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t  cond = PTHREAD_COND_INITIALIZER;
struct SPSCQueue {
     struct SPSCQueue * next;
    int num;
    int count;
    int capacity;
} typedef SPSCQueue ;
SPSCQueue *SPSCQueueInit(int capacity)
{
   struct SPSCQueue *temp= (struct SPSCQueue*)malloc(sizeof(struct SPSCQueue));
    temp->capacity=capacity;
    temp->num=0;
    temp->count=0;
    temp->next=NULL;
    return head;
}
void SPSCQueuePush(SPSCQueue *queue, void *s)
{   
    queue->num=(int)s;
    queue->next=NULL;
    printf("生产%d号产品:%d\n", i++, queue->num);
    if(head->next == NULL)
    {
        head->next=queue;
        head->count++;
    }
    else
    {
        struct SPSCQueue* tail=head;
        while(tail->next!=NULL)
        {
        tail=tail->next;
        }
        tail->next=queue;
        head->count++;
    }
}
void *SPSCQueuePop(SPSCQueue *queue)
{  
    struct SPSCQueue *cur =queue;
    while (cur->next!= NULL) //找到尾节点
    {
        cur= cur->next;
    }
    head->count--;
    printf("消费%d产品:%d\n", i--, queue->next->num);
    free(cur);
    cur= NULL;

}
void SPSCQueueDestory(SPSCQueue *queue)
{
    free(queue);
    queue->capacity=queue->num=queue->count=0;
    queue->next=NULL;
    
}
void* producer(void * b)
{
    struct SPSCQueue* sq;
    while(1)
    {
        sq =SPSCQueueInit(1);
        if(sq==NULL)
        {
            perror("malloc");
            exit(-1);
        }
        // 加锁
        pthread_mutex_lock(&mut);
        while(head->capacity == head->count)
            pthread_cond_wait(&cond,&mut);
        //生产
        SPSCQueuePush(sq, rand() % 1000 + 1) ;
        //解锁
        pthread_mutex_unlock(&mut);
        //通知
        pthread_cond_signal(&cond);
    }
    
}
void* consumer(void * a)
{
    while(1)
    {
         pthread_mutex_lock(&mut);
        //解锁
         while(head->next==NULL)
            pthread_cond_wait(&cond,&mut);
        SPSCQueuePop(head);
        pthread_mutex_unlock(&mut);
        pthread_cond_signal(&cond);
    }
}
int main()
{
    pthread_t cid,pid;
    head=SPSCQueueInit(8);
    int err1=pthread_create(&pid,NULL,producer,NULL);
     if(err1)
        {
            fprintf(stderr,"pthread_creat():%s\n",strerror(err1));
            exit(1);
        }
    int err2=pthread_create(&cid,NULL,consumer,NULL);
      if(err2)
        {
            fprintf(stderr,"pthread_creat():%s\n",strerror(err1));
            exit(1);
        }

    pthread_join(pid,NULL);
    pthread_join(cid,NULL);
}