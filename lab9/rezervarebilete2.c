#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_SELLERS 50
#define NUM_TICKETS 25000

static int numTickets = NUM_TICKETS;

pthread_mutex_t mutex;

void *sellerThread(void* arg)
{
  long total = 0;
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

  printf("Seller %ld finished! (I sold %ld tickets)\n", (long) arg, total);

  // alloc result on heap, not on stack!
  long *result = malloc(sizeof(long));
  result[0] = total;
  pthread_exit((void*)result);
}

int main(void)
{
  long total_sold = 0;

  pthread_t tids[NUM_SELLERS];

  pthread_mutex_init(&mutex, NULL);

  long i;
  for(i=0; i < NUM_SELLERS; i++) {
    pthread_create(&tids[i], NULL, sellerThread, (void*)i);
  }

  long **results = malloc(sizeof(long) * NUM_SELLERS);
  for(i=0; i < NUM_SELLERS; i++) {
    pthread_join(tids[i], (void**)&results[i]);
    total_sold += results[i][0];
    free(results[i]);
  }
  free(results);

  pthread_mutex_destroy(&mutex);

  printf("Done %ld!\n", total_sold);
  return 0;
}
