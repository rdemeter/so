#include <stdio.h>
#include <pthread.h>
#define NUM_THREADS 5
// implementarea unei bariere *nereentrante* cu variabile de conditie
struct my_barrier_t {
// mutex folosit pentru a serializa accesele la datele interne ale barierei
pthread_mutex_t lock;
// variabila de conditie pe care se astepta sosirea tuturor firelor de executie
pthread_cond_t cond;
// numar de fire de executie care trebuie sa mai vina pentru a elibera bariera
int nr_still_to_come;
};
struct my_barrier_t bar;
void my_barrier_init(struct my_barrier_t * bar, int nr_still_to_come)
{
pthread_mutex_init(&bar->lock, NULL);
pthread_cond_init(&bar->cond, NULL);
// cate fire de executie sunt asteptate la bariera.
bar->nr_still_to_come = nr_still_to_come;
}
void my_barrier_destroy(struct my_barrier_t * bar)
{
pthread_cond_destroy(&bar->cond);
pthread_mutex_destroy(&bar->lock);
}
void *thread_routine(void *arg)
{
int thd_id = (int) arg;
// inainte de a lucra cu datele interne ale barierei trebuie sa preluam mutexul
pthread_mutex_lock(&bar.lock);
printf("thd %d: before the barrier\n", thd_id);
// suntem ultimul fir de executie care a sosit la bariera?
int is_last_to_arrive = (bar.nr_still_to_come == 1);
// decrementam numarul de fire de executie asteptate la bariera
bar.nr_still_to_come --;
// cat timp mai sunt threaduri care nu au ajuns la bariera, asteptam.
while (bar.nr_still_to_come != 0)
// lockul se elibereaza automat inainte de a incepe asteptarea
pthread_cond_wait(&bar.cond, &bar.lock);
// ultimul thread ajuns la bariera va semnaliza celelalte threaduri
if (is_last_to_arrive) {
printf("let the flood in\n");
pthread_cond_broadcast(&bar.cond);
}
printf("thd %d: after the barrier\n", thd_id);
// la iesirea din functia de asteptare se preia automat mutexul, trebuie eliberat.
pthread_mutex_unlock(&bar.lock);
return NULL;
}
int main(void) {
int i;
pthread_t tids[NUM_THREADS];
my_barrier_init(&bar, NUM_THREADS);
for (i = 0; i < NUM_THREADS; i++)
pthread_create(&tids[i], NULL, thread_routine, (void *) i);
for (i = 0; i < NUM_THREADS; i++)
pthread_join(tids[i], NULL);
my_barrier_destroy(&bar);
return 0;
}

