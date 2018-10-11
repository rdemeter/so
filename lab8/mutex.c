#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 5

// mutex global
pthread_mutex_t mutex;
int global_counter = 0;
void *thread_routine(void *arg) {
	// preluam mutexul global
	pthread_mutex_lock(&mutex);
	// afisam si modificam valoarea variabilei globale 'global_counter'
	printf("Thread %d says global_counter=%d\n", (int) arg, global_counter);
	global_counter ++;
	// eliberam mutexul pentru a acorda acces altui fir de executie
	pthread_mutex_unlock(&mutex);
	return NULL;
}
int main(void) {
	int i;
	pthread_t tids[NUM_THREADS];
	// mutexul este initializat o singura data si folosit de toate firele de executie
	pthread_mutex_init(&mutex, NULL);
	// firele de executie vor executa codul functiei 'thread_routine'
	// in locul unui pointer la date utile, se trimite in ultimul argument
	// un intreg - identificatorul firului de executie
	for (i = 0; i < NUM_THREADS; i++)
	pthread_create(&tids[i], NULL, thread_routine, (void *) i);
	// asteptam ca toate firele de executie sa se termine
	for (i = 0; i < NUM_THREADS; i++)
	pthread_join(tids[i], NULL);
	// eliberam resursele mutexului
	pthread_mutex_destroy(&mutex);
	return 0;
}

