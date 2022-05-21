
# 引言
> *有五个哲学家绕着圆桌坐，每个哲学家面前有一盘面，两人之间有一支筷子，这样每个哲学家左右各有一支筷子。哲学家有2个状态，思考或者拿起筷子吃饭。如果哲学家拿到一只筷子，不能吃饭，直到拿到2只才能吃饭，并且一次只能拿起身边的一支筷子。一旦拿起便不会放下筷子直到把饭吃完，此时才把这双筷子放回原处。如果，很不幸地，每个哲学家拿起他或她左边的筷子，那么就没有人可以吃到饭了。*


 哲学家就餐问题是经典的多线程并发争抢资源的问题，5个人抢5根筷子（~~外国人提出来的问题，为毛不用叉子吃饭~~），吃饭这个行为，需要一个人拿到2根筷子这个逻辑在背后支撑，为此有几种解决方案

 # 互斥锁


## 加锁不得打断
使用互斥锁使得 一个人 拿起 两根筷子 这个操作原子化不可被打断
 为了更好的分配筷子，前人研究得 第i个人 起手拿第i 个筷子，然后在拿第i+1根
```c
void *pthilosopher(void* argv)
{
    int i=*(int*)argv;
    int left=i;
    int right=(i+1)%N;
    while(1)
    {
        printf("哲学家正在思考\n");
        sleep(rand()%4);
        printf("哲学家%d号emo了准备吃\n",i);
        //加锁对临界区进行保护
        pthread_mutex_lock(&mut);
        sem_wait(&chopsticks[left]);
        printf("哲学家%d拿起了%d号筷子,现在只有一根筷子\n",i,left);
        sem_wait(&chopsticks[right]);
        printf("哲学家%d拿起了%d号筷子\n",i,right);
        pthread_mutex_unlock(&mut);
        //结束拿起
        printf("哲学家%d现在拿了两根筷子开始吃\n",i);
        sleep(rand()%4);
        sem_post(&chopsticks[left]);//别忘了放下
        printf("哲学家%d放下了%d号筷子\n",i,left);
        sem_post(&chopsticks[right]);
        printf("哲学家%d放下了%d号筷子\n",i,right);
    }
    return NULL;
}
```
## 奇偶分割线程
奇数号的人先拿左手，再拿右手 ，偶数号 拿右手 再拿左手，因此，2和3抢3， 3和4抢4，4和5抢5，1号不用争抢

```c
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
        sleep(rand()%4);
        printf("哲学家%d号emo了准备吃\n",i);

        pthread_mutex_lock(&mut);
        sem_wait(&chopsticks[left]); //拿起筷子
        printf("哲学家%d拿起了%d号筷子,现在只有一根筷子\n",i,left);
        sem_wait(&chopsticks[right]);
        printf("哲学家%d拿起了%d号筷子\n",i,right);
        pthread_mutex_unlock(&mut);

        printf("哲学家%d现在拿了两根筷子开始吃\n",i);
        sleep(rand()%4);
        sem_post(&chopsticks[left]);
        printf("哲学家%d放下了%d号筷子\n",i,left);//归还筷子
        sem_post(&chopsticks[right]);
        printf("哲学家%d放下了%d号筷子\n",i,right);//归还筷子
        }
        else
        {
        printf("哲学家正在思考\n");
        sleep(rand()%4);
        printf("哲学家emo了准备吃\n");

        pthread_mutex_lock(&mut);
        sem_wait(&chopsticks[right]); //拿起筷子
        printf("哲学家%d拿起了%d号筷子,现在只有一根筷子\n",i,left);
        sem_wait(&chopsticks[left]);
        printf("哲学家%d拿起了%d号筷子\n",i,right);
        pthread_mutex_unlock(&mut);

        printf("哲学家%d现在拿了两根筷子开始吃\n",i);
        sleep(rand()%4);
        sem_post(&chopsticks[left]);
        printf("哲学家%d放下了%d号筷子\n",i,left);//归还筷子
        sem_post(&chopsticks[right]);
        printf("哲学家%d放下了%d号筷子\n",i,right);//归还筷子

        }

    }
    return NULL;

}
```
# 利用信号量实现伪锁
让4个人争抢5根筷子，当第4个拿起，则第5个等待前面进行释放

```c
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

        sem_wait(&m); //控制只有4个人能去争抢
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
```