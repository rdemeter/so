#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_TOTAL_BUFFERS 5
#define DATA_LENGTH 20

char buffers[NUM_TOTAL_BUFFERS];

void *producer()
{
  int i, writePt = 0;
  char data;
  for(i=0; i < DATA_LENGTH; i++)
  {
    data = 'A' + rand() % 25;
    buffers[writePt] = data;
    printf("producer: buffer[%d]=%c\n", writePt, data);
    writePt = (writePt + 1) % NUM_TOTAL_BUFFERS;
  }
}

void* consumer()
{
  int i, readPt = 0;
  char data;
  for(i=0; i < DATA_LENGTH; i++)
  {
    data = buffers[readPt];
    printf("\t\tconsumer: buffer[%d]=%c\n", readPt, data);
    readPt = (readPt + 1) % NUM_TOTAL_BUFFERS;
  }
}

int main(void) {
  pthread_t prod, cons;
  pthread_create(&prod, NULL, producer, NULL);
  pthread_create(&cons, NULL, consumer, NULL);

  pthread_join(prod, NULL);
  pthread_join(cons, NULL);

  printf("done!\n");
  return 0;
}
