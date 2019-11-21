#include <stdio.h>
#include <signal.h>

void sighandler(int signum) {
  printf("caught signal SIGINT\n");
}

int main() {
  sigset_t set;
 
  /* block all signals except SIGINT */
  sigfillset(&set);
  sigdelset(&set, SIGINT);
 
  printf("wait for SIGINT signal\n");
  /* wait for SIGINT */
  sigsuspend(&set);
  
  printf("and exit\n");
  return 0;
}
