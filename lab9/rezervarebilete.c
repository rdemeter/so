#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#define NUM_SELLERS 5  //50
#define NUM_TICKETS 25 //250000
static int numTickets = NUM_TICKETS;

void *sellerThread(void* arg)
{
  int total = 0;
  do {
    if(numTickets <= 0) break;
    numTickets--;
    total++;
    printf("Seller %ld sold one (%d left)\n", (long) arg, numTickets);
  } while(numTickets > 0);
  printf("Seller %ld finished! (I sold %d tickets)\n", (long) arg, total);
  pthread_exit((void*) total);
}
int main(void)
{
  long i;
  int total = 0;
  pthread_t tids[NUM_SELLERS];
  for(i=0; i < NUM_SELLERS; i++)
    pthread_create(&tids[i], NULL, sellerThread, (void*) i);
  for(i=0; i < NUM_SELLERS; i++) {
    int value;
    pthread_join(tids[i], (void**)&value);
    total += value;
  }
  printf("Done %d!\n", total);
  return 0;
}
