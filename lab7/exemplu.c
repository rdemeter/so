#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/* structura ce contine parametrii transmisi fiecarui thread */
struct parametri
{
  char caracter; /* caracterul afisat */
  int numar;
  /* de cate ori va fi afisat */
};

/* functia executata de thread-uri */
void* afisare_caracter(void *params)
{
  struct parametri* p = (struct parametri*) params;
  int i;
  for (i=0;i<p->numar;i++){
    printf("%c\n", p->caracter);
    sched_yield();
  }
  printf("\n");
  return NULL;
}

int main()
{
  pthread_t fir1, fir2,fir3;
  struct parametri fir1_args, fir2_args, fir3_args;
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
  fir3_args.caracter = 'z';
  fir3_args.numar = 18;
  if (pthread_create(&fir3, NULL, &afisare_caracter, &fir3_args)) {
    perror("pthread_create");
    exit(1);
  }
  /* asteptam terminarea celor doua fire de executie */
  if (pthread_join(fir1, NULL))
    perror("pthread_join");
  if (pthread_join(fir2, NULL))
    perror("pthread_join");
  if(pthread_join(fir3, NULL))
    perror("pthread_join");
  return 0;
}
