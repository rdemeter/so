// forktest.c
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char **argv) 
{
  pid_t childPID = fork();

  if (childPID < 0)
  {
    // An error occured
    fprintf(stderr, "Could not fork!\n");
    return -1;
  }
  else if (childPID == 0)
  {
    // We are in the child process
    printf("The child process is executing...\n");
    sleep(2);
  }
  else
  {
    // We are in the parent process
    if (wait(NULL) < 0) 
    {
      fprintf(stderr, "Could not wait for child!\n");
      return -1;
    }
    printf("Everything is done!\n");
  }
  return 0;
}
