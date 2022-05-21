#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
// Single-producer , single-consumer Queue
struct SPSCQueue *head=NULL,*tail=NULL;

struct SPSCQueue {
    void **queue;//存放生产数据
    int cout;
    int capcity;//存放最大生产容量
    //struct SPSCQueue* next;
    pthread_mutex_t mut;
    pthread_cond_t cond;
} typedef SPSCQueue;

SPSCQueue *SPSCQueueInit(int capacity)
{
    struct SPSCQueue* temp=malloc(sizeof(struct SPSCQueue));
    temp->capcity=capacity;
    //temp->num=0;
    //temp->next=NULL;
    pthread_mutex_init(&temp->mut,NULL);
    pthread_cond_init(&temp->cond,NULL);
}
void SPSCQueuePush(SPSCQueue *queue, void *s)
{
    

}
void *SPSCQueuePop(SPSCQueue *queue);
void SPSCQueueDestory(SPSCQueue *);

int main()
{
    pthread_t cid,pid;
    head=SPSCQueueInit(10);
}