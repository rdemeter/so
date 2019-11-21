#include <stdio.h>
#include <signal.h>

int main() {
  sigset_t set;
 
  /* block all signals except SIGINT */
  sigfillset(&set);
  sigdelset(&set, SIGINT);
 
  printf("wait for SIGINT signal\n");
  /* wait for SIGINT */
  sigsuspend(&set);
  
  printf("Caught signal SIGINT and exit\n");
  return 0;
}
