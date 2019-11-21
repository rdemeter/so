#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

static void sighandler(int signum) {
  /* actions that should be taken when the signal signum is received */
  printf(" Caught signal %d\n", signum);
  
  //exit(1);
}

int main(void) {
  signal(SIGINT, sighandler);

  while(1) {
    printf("Going to sleep for a second...\n");
    sleep(1);
  }
  return 0;
}
