#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
// Single-producer , single-consumer Queue
int i=0;
struct SPSCQueue *head,*tail;
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
   struct SPSCQueue *temp=malloc(sizeof(struct SPSCQueue));
    temp->capacity=capacity;
    temp->num=0;
    temp->count=0;
    temp->next=NULL;
    return temp;
}
void SPSCQueuePush(SPSCQueue *queue, void *s)
{   

    queue->num=(int)s;
    queue->next=NULL;
    printf("%ld线程生产:%d\n", pthread_self(), queue->num);
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
   
    if(head->next==NULL)
    {
        printf(" %ld线程消费:%d\n", pthread_self(), head->next->next->num);
         head->count--;
        free(head->next);
        head->next->next=NULL;
    }
    else
    {
        struct SPSCQueue *next1=head->next->next;
         printf(" %ld线程消费:%d\n", pthread_self(), head->next->num);
         head->count--;
        free(head->next);
        head->next=NULL;
        head->next=next1;

        
    }

}
void SPSCQueueDestory(SPSCQueue *queue)
{
    free(queue);
    queue->capacity=queue->num=queue->count=0;
    queue->next=NULL;
    
}
void* producer(void * b)
{

    while(1)
    {
         struct SPSCQueue* sq;
        sq =SPSCQueueInit(1);
        if(sq==NULL)
        {
            perror("malloc");
            exit(-1);
        }
        // 加锁
       pthread_mutex_lock(&mut);
       while (head->count==head->capacity)
       {
        pthread_cond_wait(&cond,&mut);
       }
        //生产
        SPSCQueuePush(sq, (void*)(rand()%1000)) ;
        //解锁
       pthread_mutex_unlock(&mut);
        //通知
       pthread_cond_signal(&cond);
    }
    return NULL;
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
        pthread_cond_broadcast(&cond);
    }
}
int main()
{
    pthread_t cid[5],pid[5];
    head=SPSCQueueInit(10);

    for(i=0 ; i< 5 ;i++)
    {  
     int err1=pthread_create(&pid[i],NULL,producer,NULL);
     if(err1){
        fprintf(stderr,"pthread_creat():%s\n",strerror(err1));
        exit(1);
    }
    }
    for(i=0 ; i<5 ;i++)
    {
    int err2=pthread_create(&cid[i],NULL,consumer,NULL);
      if(err2)
        {
            fprintf(stderr,"pthread_creat():%s\n",strerror(err2));
            exit(1);
        }
    }
    for(i=0;i<5;i++)
    {
        pthread_join(pid[i],NULL);
        pthread_join(cid[i],NULL);
    }
  
    pthread_mutex_destroy(&mut);
    SPSCQueueDestory(head);
    exit(0);
}