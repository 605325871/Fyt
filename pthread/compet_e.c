#include <stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<string.h>
#define left 30000000
#define right 30000200

#define thtnum (right - left +1)
struct thr_arg_st
{
    int n;
};

void *thr_prime(void *p)
{
    int i;
    i = ((struct thr_arg_st*)p)->n; //括号
    free(p);
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
        printf("%d is a primer\n",i);
    }
   pthread_exit(NULL);
}

int main()
{
    int i,j,k;
    int err;
    pthread_t tid[thtnum];
    struct thr_arg_st *p;

    //main 线程 进行创建线程
    for(i=left ; i<=right ;i++)
    {
        p = malloc(sizeof(*p));
        if(p==NULL)
        {
            perror("malloc");
            exit(-1);
        }
        p->n=i;
        err =pthread_create(tid + (i - left),NULL,thr_prime,p);
        if(err)
        {
            fprintf(stderr,"pthread_creat():%s\n",strerror(err));
            exit(1);
        }

    }
    
    //为线程收尸
    for(i =left ; i<=right ;i++)
    {
        pthread_join(tid[i -left],NULL);
    }

    exit(0);

}