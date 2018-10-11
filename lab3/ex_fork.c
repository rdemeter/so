#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
  pid_t pid;
  switch (pid = fork())
  {
    case -1: /* fork failed */
      printf("fork failed\n");
      exit(-1);
    case 0: /* child starts executing here */
    default: /* parent starts executing here */
      printf("created process with pid %d\n", getpid());
  }
  getchar();
}
