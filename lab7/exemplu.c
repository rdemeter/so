#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/* structura ce contine parametrii transmisi fiecarui thread */
struct parametri
{
  char caracter; /* caracterul afisat */
  int numar;  /* de cate ori va fi afisat */
};

/* functia executata de thread-uri */
void* afisare_caracter(void *params)
{
  struct parametri* p = (struct parametri*) params;
  int i;
  for (i=0; i<p->numar; i++){
    printf("%c\n", p->caracter);
    //sched_yield();
  }
  printf("\n");
  return NULL;
}

int main()
{
  pthread_t fir1, fir2;
  struct parametri fir1_args, fir2_args;

  /* cream un thread care va afisa 'x' de 11 ori */
  fir1_args.caracter = 'x';
  fir1_args.numar = 11;
  if (pthread_create(&fir1, NULL, &afisare_caracter, &fir1_args)) {
    perror("pthread_create");
    exit(1);
  }

  /* cream un thread care va afisa 'y' de 13 ori */
  fir2_args.caracter = 'y';
  fir2_args.numar = 13;
  if (pthread_create(&fir2, NULL, &afisare_caracter, &fir2_args)) {
    perror("pthread_create");
    exit(1);
  }

  /* wait for a thread to terminate */
  if (pthread_join(fir1, NULL))
    perror("pthread_join");
  if (pthread_join(fir2, NULL))
    perror("pthread_join");

  return 0;
}
