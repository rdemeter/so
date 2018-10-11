#define _XOPEN_SOURCE 600
#include <pthread.h>
#include <stdio.h>
pthread_barrier_t barrier;
#define NUM_THREADS 5
void *thread_routine(void *arg)
{
int thd_id = (int) arg;
int rc;
printf("thd %d: before teh barrier\n", thd_id);
// toate firele de executie asteapta la bariera.
rc = pthread_barrier_wait(&barrier);
if (rc == PTHREAD_BARRIER_SERIAL_THREAD) {
// un singur fir de execuie (posibil ultimul) va intoarce PTHREAD_BARRIER_SERIAL_THREAD
// restul firelor de execuie întorc 0 în caz de succes.
printf("thd %d let the flood in\n", thd_id);
}
printf("thd %d: after the barrier\n", thd_id);
return NULL;
}
int main()
{
int i;
pthread_t tids[NUM_THREADS];
// bariera este initializata o singura data si folosita de toate firele de executie
pthread_barrier_init(&barrier, NULL, NUM_THREADS);
// firele de executie vor executa codul functiei 'thread_routine'
// in locul unui pointer la date utile, se trimite in ultimul argument
// un intreg - identificatorul firului de executie
for (i = 0; i < NUM_THREADS; i++)
pthread_create(&tids[i], NULL, thread_routine, (void *) i);
// asteptam ca toate firele de executie sa se termine
for (i = 0; i < NUM_THREADS; i++)
pthread_join(tids[i], NULL);
// eliberam resursele barierei
pthread_barrier_destroy(&barrier);
return 0;
}

