/* În acest exemplu, thread1 încearcă să blocheze resursa 1 și apoi resursa 2, în timp ce thread2 încearcă
să blocheze resursa 2 și apoi resursa 1. Dacă cele două thread-uri rulează în paralel, există posibilitatea
ca ele să blocheze una pe cealaltă și să ajungă într-o stare de deadlock, în care niciuna dintre ele nu poate face progres */

#include <stdio.h>
#include <pthread.h>

pthread_mutex_t resource1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t resource2 = PTHREAD_MUTEX_INITIALIZER;

void *thread1()
{
    pthread_mutex_lock(&resource1);
    printf("Thread 1: Locked resource 1\n");
    
    // Acest delay simulază procesarea resursei 1
    sleep(1);
    
    pthread_mutex_lock(&resource2);
    printf("Thread 1: Locked resource 2\n");
    
    // Procesarea resursei 2
    
    pthread_mutex_unlock(&resource2);
    printf("Thread 1: Unlocked resource 2\n");
    
    pthread_mutex_unlock(&resource1);
    printf("Thread 1: Unlocked resource 1\n");
    
    return NULL;
}

void *thread2() 
{
    pthread_mutex_lock(&resource2);
    printf("Thread 2: Locked resource 2\n");
    
    // Acest delay simulază procesarea resursei 2
    sleep(1);
    
    pthread_mutex_lock(&resource1);
    printf("Thread 2: Locked resource 1\n");
    
    // Procesarea resursei 1
    
    pthread_mutex_unlock(&resource1);
    printf("Thread 2: Unlocked resource 1\n");
    
    pthread_mutex_unlock(&resource2);
    printf("Thread 2: Unlocked resource 2\n");
    
    return NULL;
}

int main()
{
    pthread_t t1, t2;
    pthread_create(&t1, NULL, thread1, NULL);
    pthread_create(&t2, NULL, thread2, NULL);
    
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    
    return 0;
}
