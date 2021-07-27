#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
 
#define N 5
int a = 30;   
sem_t chopsticks[N]={1,1,1,1,1};//设置5种互斥的变量
 
pthread_mutex_t mutex;//定义互斥锁
 
int philosophers[N] = {0, 1, 2, 3, 4};//定义5位哲学家
 
void delay (int len) //定义随机延迟时长
{
	int i = rand() % len;
	int x;
	while (i > 0) 
	{
		x = rand() % len;
		while (x > 0) 
		{
			x--;
		}
		i--;
	}
}



void *philosopher (void* arg) 
{
	int i = *(int *)arg;
	int left = i;//左筷子的编号和哲学家的编号相同
	int right = (i + 1) % N;//右筷子的编号为哲学家编号+1 
	
	for(a;a>0;a--)
	{
		printf("哲学家%d正在思考\n", i);
		delay(1000);
		
		printf("哲学家%d饿了\n", i);
		delay(10000);
 
		pthread_mutex_lock(&mutex);//上锁
 
		sem_wait(&chopsticks[left]);//此时这个哲学家有左筷子才考虑右筷子
		printf("哲学家%d拿起了%d号筷子,筷子只有一支,不能吃饭\n", i, left);
		sem_wait(&chopsticks[right]);
		printf("哲学家%d拿起了%d号筷子\n", i, right);
		delay(20000);
 
		pthread_mutex_unlock(&mutex);//解锁
 
		printf("哲学家%d有两支筷子,开饭\n", i);
		delay(30000);
		sem_post(&chopsticks[left]);
		printf("哲学家%d放下了%d号筷子\n", i, left);
		sem_post(&chopsticks[right]);
		printf("哲学家%d放下了%d号筷子\n", i, right);
		delay(40000);	

	}

	

		
}
int main (int argc, char **argv) 
{
	srand(time(NULL));
	pthread_t philo[N];
	
	//信号量初始化
	for (int i=0; i<N; i++) 
	{
		sem_init(&chopsticks[i], 0, 1);
	} 
	
	
	pthread_mutex_init(&mutex,NULL);//初始化互斥
	
	//创建线程
	for (int i=0; i<N; i++) 
	{
		pthread_create(&philo[i], NULL, philosopher, &philosophers[i]);
	}
	
	//挂起线程
	for (int i=0; i<N; i++) 
	{
		pthread_join(philo[i], NULL);
	}
	
	//销毁信号量
	for (int i=0; i<N; i++) 
	{
		sem_destroy(&chopsticks[i]);
	}
 
	pthread_mutex_destroy(&mutex);//销毁互斥锁
 
	return 0;
}

