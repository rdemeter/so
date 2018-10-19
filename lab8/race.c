#include <pthread.h>
#include <stdio.h>
#define INC_LIMIT 50000
#define THREADS_NO 100

int a = 0, b = 0;
void* single_increment(void* arg)
{
  int i;
  for(i = 0; i < INC_LIMIT; i++)
    a++;
  return NULL;
}

void* double_increment(void* arg)
{
  int i;
  for(i = 0; i < INC_LIMIT; i++) {
    b++;
    b++;
  }
  return NULL;
}

int main()
{
  int i;
  pthread_t sinc_threads[THREADS_NO];
  pthread_t dinc_threads[THREADS_NO];

  for(i = 0; i < THREADS_NO; i++) {
    pthread_create(&sinc_threads[i], NULL, single_increment, NULL);
    pthread_create(&dinc_threads[i], NULL, double_increment, NULL);
  }

  for(i = 0; i < THREADS_NO; i++) {
    pthread_join(sinc_threads[i], NULL);
    pthread_join(dinc_threads[i], NULL);
  }

  printf("Single Incremented variable: got %d expected %d\n", a, INC_LIMIT*THREADS_NO);
  printf("Double incremented variable: got %d expected %d\n", b, 2 * INC_LIMIT*THREADS_NO);
  return 0;
}
