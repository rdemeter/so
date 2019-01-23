#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_SELLERS 5
#define NUM_TICKETS 25000

static int numTickets = NUM_TICKETS;

int total_sold = 0;

pthread_mutex_t mutex;

void *sellerThread(void* arg)
{
  int total = 0;
  do {
    pthread_mutex_lock(&mutex);
    if(numTickets <= 0) {
        pthread_mutex_unlock(&mutex);
        break;
    }
    numTickets--;

    total++;

    printf("Seller %ld sold one (%d left)\n", (long) arg, numTickets);
    pthread_mutex_unlock(&mutex);
  } while(numTickets > 0);

  printf("Seller %ld finished! (I sold %d tickets)\n", (long) arg, total);

  // alloc result on heap, not on stack!
  int *result = malloc(sizeof(int));
  result[0] = total;
  pthread_exit((void*)result);
}

int main(void)
{
  long i;

  pthread_t tids[NUM_SELLERS];

  pthread_mutex_init(&mutex, NULL);

  for(i=0; i < NUM_SELLERS; i++) {
    pthread_create(&tids[i], NULL, sellerThread, (void*)i);
  }

  int **results = malloc(sizeof(int) * NUM_SELLERS);
  for(i=0; i < NUM_SELLERS; i++) {
    pthread_join(tids[i], (void**)&results[i]);
    total_sold += results[i][0];
    free(results[i]);
  }
  free(results);

  pthread_mutex_destroy(&mutex);

  printf("Done %d!\n", total_sold);
  return 0;
}
