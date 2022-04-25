#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<stdlib.h>
#include<string.h>

int val=0;

static void *Routine(void* args)
{
    while(1)
    {
        printf("i am %s...val = %d\n",(char*)args,val++);
        sleep(1);
    }
}

int main()
{
    pthread_t t1,t2,t3;
    
    int re1 = pthread_create(&t1,NULL,Routine,(void*)"pthread 1");
    if( re1 != 0)
    {
        fprintf(stderr,"pthread_creat:%s\n",Routine,strerror(re1));
        exit(1);
    }
    int re2 = pthread_create(&t2,NULL,Routine,(void*)"pthread 2");
    if( re2 != 0)
    {
        fprintf(stderr,"pthread_creat:%s\n",Routine,strerror(re2));
        exit(1);
    }
     int re3 = pthread_create(&t3,NULL,Routine,(void*)"pthread 3");
    if( re3 != 0)
    {
        fprintf(stderr,"pthread_creat:%s\n",Routine,strerror(re3));
        exit(1);
    }

    while(1)
    {
        printf("i am main pthread...val:%d\n",val);
        sleep(1);
    }
    return 0;
}