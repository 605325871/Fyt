#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <semaphore.h>
# define N 5
sem_t chopsticks[N];
pthread_mutex_t mut ;
int philosophers[N] ={0,1,2,3,4};

/*规定奇数号哲学家先拿起左边再右边，偶数号哲学家与之相反*/
void* phlos(void*arg)
{
    int i=*(int*)arg;
    int left=i;
    int right=(i+1 )%N;
    while(1)
    {
        if(0 == i%2)
        {
        printf("哲学家正在思考\n");
        // sleep(rand()%4);
        printf("哲学家%d号emo了准备吃\n",i);

        pthread_mutex_lock(&mut);
        sem_wait(&chopsticks[left]); //拿起筷子
        printf("哲学家%d拿起了%d号筷子,现在只有一根筷子\n",i,left);
        // sem_wait(&chopsticks[right]);
        printf("哲学家%d拿起了%d号筷子\n",i,right);
        pthread_mutex_unlock(&mut);

        printf("哲学家%d现在拿了两根筷子开始吃\n",i);
        // sleep(rand()%4);
        sem_post(&chopsticks[left]);
        printf("哲学家%d放下了%d号筷子\n",i,left);//归还筷子
        sem_post(&chopsticks[right]);
        printf("哲学家%d放下了%d号筷子\n",i,right);//归还筷子
        }
        else
        {
        printf("哲学家正在思考\n");
        // sleep(rand()%4);
        printf("哲学家emo了准备吃\n");

        pthread_mutex_lock(&mut);
        sem_wait(&chopsticks[right]); //拿起筷子
        printf("哲学家%d拿起了%d号筷子,现在只有一根筷子\n",i,left);
        sem_wait(&chopsticks[left]);
        printf("哲学家%d拿起了%d号筷子\n",i,right);
        pthread_mutex_unlock(&mut);

        printf("哲学家%d现在拿了两根筷子开始吃\n",i);
        // sleep(rand()%4);
        sem_post(&chopsticks[left]);
        printf("哲学家%d放下了%d号筷子\n",i,left);//归还筷子
        sem_post(&chopsticks[right]);
        printf("哲学家%d放下了%d号筷子\n",i,right);//归还筷子

        }

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
    pthread_mutex_init(&mut,NULL);
    for(i=0;i<N;i++){
        pthread_create(&phio[i],NULL,phlos,&philosophers[i]);
    }
     for(i=0;i<N;i++){
        pthread_join(phio[i],NULL);
    }
    for(i=0;i<N;i++){
        sem_destroy(&chopsticks[i]);
    }
    pthread_mutex_destroy(&mut);

}