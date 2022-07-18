#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <semaphore.h>
# define N 5
sem_t chopsticks[N];// 筷子的数量
int philosophers[N] ={1,2,3,4,5};//哲学家
sem_t m;


/* 使用互斥锁保护当哲学家拿起了两根筷子才可以进参
*/
void *pthilosopher(void* argv)
{
    int i=*(int*)argv;
    int left=i;
    int right=(i+1)%N;
    while(1)
    {
        printf("哲学家%d号正在思考\n",i);
        sleep(rand()%4);
        printf("哲学家%d号emo了准备吃\n",i);

        sem_wait(&m);
        sem_wait(&chopsticks[left]);
        printf("哲学家%d拿起了%d号筷子,现在只有一根筷子\n",i,left);
        sem_wait(&chopsticks[right]);
        printf("哲学家%d拿起了%d号筷子\n",i,right);
        
        printf("哲学家%d现在拿了两根筷子开始吃\n",i);
        sleep(rand()%4);
        sem_post(&chopsticks[left]);
        printf("哲学家%d放下了%d号筷子\n",i,left);
        sem_post(&chopsticks[right]);

        sem_post(&m);
        printf("哲学家%d放下了%d号筷子\n",i,right);
    }
    return NULL;
}
int main()
{
    pthread_t phio[N];
    int i;
    for(i=0;i<N;i++){
        sem_init(&chopsticks[i],0,1);
    }
    sem_init(&m,0,4);
    for(i=0;i<N;i++){
        pthread_create(&phio[i],NULL,pthilosopher,&philosophers[i]);
    }
     for(i=0;i<N;i++){
        pthread_join(phio[i],NULL);
    }
    for(i=0;i<N;i++){
        sem_destroy(&chopsticks[i]);
    }
    sem_destroy(&m);
    return 0;

}