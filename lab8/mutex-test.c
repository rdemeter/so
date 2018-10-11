#include <stdio.h>
#include <sched.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t count_lock;
pthread_cond_t count_nonzero;
unsigned count;
void *decrement_count() {
while(1)
{
pthread_mutex_lock(&count_lock);
if (count == 0)
pthread_cond_wait(&count_nonzero, &count_lock);
count = count - 1;
printf("dec count=%d\n", count);
pthread_mutex_unlock(&count_lock);
}
}
void *increment_count() {
while(1)
{
pthread_mutex_lock(&count_lock);
count = count + 1;
printf("inc count=%d\n", count);
if (count > 2) {
printf("signal ...\n");
pthread_cond_signal(&count_nonzero);
}
pthread_mutex_unlock(&count_lock);
}
}
int main() {
pthread_t dec, inc;
pthread_create(&dec, NULL, decrement_count, NULL);
pthread_create(&inc, NULL, increment_count, NULL);
pthread_join(inc, NULL);
pthread_join(dec, NULL);
return 0;
}

