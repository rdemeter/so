#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int main() {
  sigset_t set;
 
 sigemptyset(&set);
 sigaddset(&set, SIGINT);
 
  while (1) {
    sleep(5);
    printf("block SIGINT\n");
    sigprocmask(SIG_BLOCK, &set, NULL);
    sleep(5);
    printf("unblock SIGINT\n");
    sigprocmask(SIG_UNBLOCK, &set, NULL);
  }
  return 0;
}
