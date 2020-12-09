#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

void* thread_function(void)
{
  char *a = malloc(12);
  strcpy(a, "hello world");
  //sleep(10);
  pthread_exit((void*)a);
}

int main()
{
  pthread_t thread_id;
  char *b = NULL;
  pthread_create (&thread_id, NULL, (void*)&thread_function, NULL);

  //
  //While the created thread is running, the main thread can perform other tasks.
  
  //here we are reciving one pointer value so to use that we need double pointer
  pthread_join(thread_id,(void**)&b);

  printf("b is %s\n", b);
  free(b); // lets free the memory
}
