#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>

void* thread_function(void)
{
  char *a = malloc(12);
  strcpy(a,"hello world");
  pthread_exit((void*)a);
}

int main()
{
  pthread_t thread_id;
  char *b;
  pthread_create (&thread_id, NULL, &thread_function, NULL);

  //here we are reciving one pointer value so to use that we need double pointer
  pthread_join(thread_id,(void**)&b);

  printf("b is %s\n", b);
  free(b); // lets free the memory
}
