# include<pthread.h>
# include <stdio.h>
# include<stdlib.h>
# include<pthread.h>
# include<string.h>
# include "tlpi_hdr.h"
static void * threadfunc(void* arg)
{
    char *s =(char*)arg;
    printf("%s",s);

    return (void*)strlen(s);

}

int main()
{
    pthread_t tid;
    void*res;
    int s;
    char *ch="hello world\n";
    s = pthread_create(&tid,NULL,threadfunc,ch);
    // if(s != 0)
    // errExitEN(s,"pthread_create");

    printf("message from main()");

    s = pthread_join(tid,&res);

    // if(s != 0)
    //     errExitEN(s,"pthread_join");
    printf("thread returned %ld\n",(long)res);
    exit(EXIT_SUCCESS);

}