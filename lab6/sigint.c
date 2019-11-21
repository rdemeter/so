#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<signal.h>

static void sigint_handler(int signum) {
  /* actions that should be taken when the signal signum is received */
  printf(" received sigint!!!\n");
}

int main(void) {
  struct sigaction sa;
  memset(&sa, 0, sizeof(sa));

  sa.sa_flags = SA_RESTART;
  sa.sa_handler = sigint_handler;
  sigaction(SIGINT, &sa, NULL);

  while(1) {
    sleep(1);
  }
  return 0;
}
