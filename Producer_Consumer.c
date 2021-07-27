#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#define PRO_NUM 3          // �����ߵ���Ŀ
#define CON_NUM 4          // �����ߵ���Ŀ
#define BUFF_SIZE 10       // ����
int in = 0;                // �����߷��ò�Ʒ��λ��
int out = 0;               // ������ȡ��Ʒ��λ��
int buff[BUFF_SIZE] = {0}; // �����ʼ��Ϊ0�� ��ʼʱû�в�Ʒ
sem_t empty_sem;           // ͬ���ź����� ������ʱ��ֹ�����߷Ų�Ʒ
sem_t full_sem;            // ͬ���ź����� ��û��Ʒʱ��ֹ����������
pthread_mutex_t mutex;     // �����ź����� һ��ֻ��һ���̷߳��ʻ���
int pro_id = 0;            //������id
int con_id = 0;            //������id
FILE *fp;
/* �����߷��� */
void *producer()
{
   int id = ++pro_id;
   while (1)
   {
      sleep(1);
      sem_wait(&empty_sem);
      pthread_mutex_lock(&mutex);
      in = in % BUFF_SIZE;
      buff[in] = fgetc(fp);
      printf("producer%d has produced %d !\n", id, buff[in]);
      in++;
      pthread_mutex_unlock(&mutex);
      sem_post(&full_sem);
   }
}

/* �����߷��� */
void *consumer()
{
   int id = ++con_id;
   while (1)
   {
      sleep(1);
      sem_wait(&full_sem);
      pthread_mutex_lock(&mutex);
      out = out % BUFF_SIZE;
      printf("consumer%d has consumed %d !\n", id, buff[out]);
      buff[out] = 0;
      out++;
      pthread_mutex_unlock(&mutex);
      sem_post(&empty_sem);
   }
}

int main()
{
   fp = fopen("test2.txt", "r");
   pthread_t id1[PRO_NUM];
   pthread_t id2[CON_NUM];
   int i;
   int ret_pro[PRO_NUM];
   int ret_con[CON_NUM];

   // ��ʼ��ͬ���ź���
   int init1 = sem_init(&empty_sem, 0, BUFF_SIZE);
   int init2 = sem_init(&full_sem, 0, 0);
   if (init1 && init2 != 0)
   {
      printf("sem init failed \n");
      exit(1);
   }
   //��ʼ�������ź���
   int init3 = pthread_mutex_init(&mutex, NULL);
   if (init3 != 0)
   {
      printf("mutex init failed \n");
      exit(1);
   }
   // ����NUM���������߳�
   for (i = 0; i < PRO_NUM; i++)
   {
      ret_pro[i] = pthread_create(&id1[i], NULL, producer, (void *)(&i));
      if (ret_pro[i] != 0)
      {
         printf("producer%d create failed \n", i);
         exit(1);
      }
   }
   //����NUM���������߳�
   for (i = 0; i < CON_NUM; i++)
   {
      ret_con[i] = pthread_create(&id2[i], NULL, consumer, NULL);
      if (ret_con[i] != 0)
      {
         printf("consumer%d create failed \n", i);
         exit(1);
      }
   }
   //�����߳�
   for (i = 0; i < PRO_NUM; i++)
   {
      pthread_join(id1[i], NULL);
   }
   //�����߳�
   for (i = 0; i < CON_NUM; i++)
   {
      pthread_join(id2[i], NULL);
   }
   exit(0);
   fclose(fp);
}
