#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_TOTAL_BUFFERS 5
#define DATA_LENGTH 20

char buffers[NUM_TOTAL_BUFFERS];
sem_t empty, full;
pthread_mutex_t mutex;

void *producer()
{
  int i, writePt = 0;
  char data;

  for(i=0; i < DATA_LENGTH; i++)
  {
    sem_wait(&empty);
    pthread_mutex_lock(&mutex);
    data = 'A' + rand() % 25;
    buffers[writePt] = data;
    printf("producer: buffer[%d]=%c\n", writePt, data);
    writePt = (writePt + 1) % NUM_TOTAL_BUFFERS;
    pthread_mutex_unlock(&mutex);
    sem_post(&full);
  }
}

void* consumer()
{
  int i, readPt = 0;
  char data;

  for(i=0; i < DATA_LENGTH; i++)
  {
    sem_wait(&full);
    pthread_mutex_lock(&mutex);
    data = buffers[readPt];
    printf("\t\tconsumer: buffer[%d]=%c\n", readPt, data);
    readPt = (readPt + 1) % NUM_TOTAL_BUFFERS;
    pthread_mutex_unlock(&mutex);
    sem_post(&empty);
  }
}

int main(void)
{
  sem_init(&full, 0, 0);
  sem_init(&empty, 0, NUM_TOTAL_BUFFERS);

  pthread_t prod, cons;
  pthread_create(&prod, NULL, producer, NULL);
  pthread_create(&cons, NULL, consumer, NULL);

  pthread_join(prod, NULL);
  pthread_join(cons, NULL);

  sem_destroy(&full);
  sem_destroy(&empty);

  printf("done!\n");
  return 0;
}
