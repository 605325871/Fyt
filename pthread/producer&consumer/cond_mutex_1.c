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