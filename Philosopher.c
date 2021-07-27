#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define N 5
int count = 30;
sem_t chopsticks[N] = {1, 1, 1, 1, 1}; //设置5种互斥的变量

pthread_mutex_t mutex; //定义互斥锁

int philosophers[N] = {0, 1, 2, 3, 4}; //定义5位哲学家
void *philosopher(void *arg)
{
	int i = *(int *)arg;
	int left = i;			 //左边筷子编号
	int right = (i + 1) % N; //右边筷子编号

	for (count; count > 0; count--)
	{
		printf("哲学家%d正在思考\n", i);
		sleep(1);
		pthread_mutex_lock(&mutex);	 //上锁
		sem_wait(&chopsticks[left]); //先试图拿左边筷子
		printf("哲学家%d拿起了%d号（左）筷子\n", i, left);
		sem_wait(&chopsticks[right]);
		printf("哲学家%d拿起了%d号（右）筷子\n", i, right);
		sleep(1);
		pthread_mutex_unlock(&mutex); //解锁
		printf("哲学家%d有两支筷子,开始进餐\n", i);
		sleep(1);
		sem_post(&chopsticks[left]);
		printf("哲学家%d放下了%d号（左）筷子\n", i, left);
		sem_post(&chopsticks[right]);
		printf("哲学家%d放下了%d号（右）筷子\n", i, right);
		sleep(1);
	}
}
int main(int argc, char **argv)
{
	srand(time(NULL));
	pthread_t philo[N];

	//初始化信号量
	for (int i = 0; i < N; i++)
	{
		sem_init(&chopsticks[i], 0, 1);
	}
	pthread_mutex_init(&mutex, NULL); //初始化互斥锁
	//创建线程
	for (int i = 0; i < N; i++)
	{
		pthread_create(&philo[i], NULL, philosopher, &philosophers[i]);
	}
	//挂起线程
	for (int i = 0; i < N; i++)
	{
		pthread_join(philo[i], NULL);
	}
	//销毁信号量
	for (int i = 0; i < N; i++)
	{
		sem_destroy(&chopsticks[i]);
	}
	pthread_mutex_destroy(&mutex); //销毁互斥锁
	return 0;
}
