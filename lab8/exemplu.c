#include <stdio.h>
#include <pthread.h>
#define NUM_THREADS 5
// mutex global
pthread_mutex_t mutex;
int global_counter = 0;
void *thread_routine( )
{
	pthread_mutex_lock(&mutex);
	pthread_mutex_lock(&mutex);

	printf("am ajuns in zona protejata de mutex\n");

	pthread_mutex_unlock(&mutex);
	pthread_mutex_unlock(&mutex);
}
int main(void) {
int i;
pthread_t tids[NUM_THREADS];


// definim atribute, le initializam si marcam tipul ca fiind recursiv.
pthread_mutexattr_t attr;
pthread_mutexattr_init(&attr);
pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
// definim un mutex recursiv, il initializam cu atributele definite anterior

pthread_mutex_init(&mutex, &attr);

pthread_mutexattr_destroy(&attr);


// firele de executie vor executa codul functiei 'thread_routine'
// in locul unui pointer la date utile, se trimite in ultimul argument
// un intreg - identificatorul firului de executie
for (i = 0; i < NUM_THREADS; i++)
pthread_create(&tids[i], NULL, thread_routine, (void *) i);
// asteptam ca toate firele de executie sa se termine
for (i = 0; i < NUM_THREADS; i++)
pthread_join(tids[i], NULL);
// eliberam resursele mutexului

// eliberare mutex
pthread_mutex_destroy(&mutex);
return 0;
}

