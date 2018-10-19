#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

#define DATA_LENGTH 20
#define NR_WRITER 5
#define NR_READER 10

char buffers[DATA_LENGTH];
pthread_mutex_t mutex;
pthread_mutex_t db;
int rc = 0;

void *writer(void* arg)
{
  int i, writePt = 0;
  char data;

  for(i=0; i < DATA_LENGTH; i++)
  {
    pthread_mutex_lock(&db);
    printf("writing#%ld\n", (long) arg);
    data = 'A' + rand() % 25;
    buffers[writePt] = data;
    sleep(rand()%5);
    printf("write#%ld: buffer[%d]=%c\n", (long) arg, writePt, data);
    writePt = (writePt + 1);
    pthread_mutex_unlock(&db);
  }
}

void* reader(void* arg)
{
  int i, readPt = 0;
  char data;

  for(i=0; i < DATA_LENGTH; i++)
  {
    pthread_mutex_lock(&mutex);
    rc = rc + 1;
    if(rc == 1) pthread_mutex_lock(&db);
    pthread_mutex_unlock(&mutex);
    printf("\t\treading#%ld\n", (long) arg);
    sleep(rand()%3);
    data = buffers[readPt];
    printf("\t\tread#%ld: buffer[%d]=%c\n", (long) arg, readPt, data);
    readPt = (readPt + 1);
    pthread_mutex_lock(&mutex);
    rc = rc - 1;
    if(rc == 0) pthread_mutex_unlock(&db);
    pthread_mutex_unlock(&mutex);
  }
}

int main(void)
{
  long i;
  pthread_t wrt[NR_WRITER], rd[NR_READER];

  pthread_mutex_init(&mutex, NULL);
  pthread_mutex_init(&db, NULL);

  for(i = 0; i < NR_WRITER; i++)
    pthread_create(&wrt[i], NULL, writer, (void*) i);
  for(i = 0; i < NR_READER; i++)
    pthread_create(&rd[i], NULL, reader, (void*) i);

  for(i = 0; i < NR_WRITER; i++)
    pthread_join(wrt[i], NULL);
  for(i = 0; i < NR_READER; i++)
    pthread_join(rd[i], NULL);

  pthread_mutex_destroy(&mutex);
  pthread_mutex_destroy(&db);

  printf("done!\n");
  return 0;
}
