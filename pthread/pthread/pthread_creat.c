# include <stdio.h>
# include<stdlib.h>
# include<pthread.h>

static void* func(void* p)
{
  puts("thread is working");
  return NULL;
}

int main()
{
   pthread_t tid;
   int err;
   puts("begin!");
   err = pthread_create(&tid,NULL,func,NULL);
   if(err)
   {
    exit(-1);
   }
   pthread_join(tid,NULL);
   puts("end!");
   exit(0);
}